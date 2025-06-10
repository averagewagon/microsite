---
title: "Serving this Blog at 240 MHz"
date: "2025-03-04"
description:
  "Why are web servers so complex and expensive when all they do is exchange
  text? This article covers the technical design of
  [joni-on-micro.site](https://joni-on-micro.site), my webserver and personal
  blog site and which is hosted on a 13 dollar ESP32-S3 with performance that's
  downright reasonable."
tags:
  - Microsite
  - Embedded
  - Web
  - Projects
---

This website is hosted on a single 13 dollar microcontroller in my studio
apartment.

This article covers the technical design of
[joni-on-micro.site](https://joni-on-micro.site), my webserver and personal blog
site and which is hosted on an ESP32-S3 with performance that's quite
reasonable.

A web server is a computer connected to the internet, usually available through
an easily-remembered domain. When your web browser loads a typical modern
webpage, it sends an HTTPS request to the web server hosting that page. The
server then processes this request and sends back the necessary files, such as
HTML, CSS, JavaScript, and images, which your browser uses to display the
webpage on your screen.

At the high end, web servers might be entire fleets of rack-mounted computers,
located in different countries and costing millions upon millions of dollars. At
the low end, a web server might be a single Raspberry Pi in somebody's
apartment.

(Image showing the ESP32-S3 next to a banana or soda can or something for scale)

The blog you're currently reading is hosted on an ESP32-S3-DevKitC-1-N32R8V,
with far less memory and computing power than any desktop-grade computer. Even a
Raspberry Pi 5, a common lower-end computer, is much more powerful than this
particular development board.

| Specification                 | ESP32-S3-DevKitC-1-N32R8V                                                                                       | Raspberry Pi 5                                                                                           |
| ----------------------------- | --------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| **Cost (at time of writing)** | $13 ([DigiKey](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-DEVKITC-1U-N8R8/16162636)) | $50-130 depending on RAM ([SparkFun](https://www.sparkfun.com/raspberry-pi-5-16gb.html?src=raspberrypi)) |
| **Processor**                 | 2 cores at 240 MHz                                                                                              | 4 cores at 2.4 GHz                                                                                       |
| **RAM**                       | 512 KB SRAM, 8 MB PSRAM                                                                                         | 2 to 16 GB                                                                                               |
| **Flash Storage**             | 32 MB                                                                                                           | MicroSD card slot                                                                                        |
| **Wi-Fi**                     | 2.4 GHz                                                                                                         | 2.4 GHz and 5 GHz                                                                                        |

Despite the resource constraints, the microcontroller does an adequate job of
delivering this website to your computer, phone, or smart fridge. A web server
with these specs won't be serving millions of visitors, but most web servers
don't serve that many people anyways.

Microsite's benchmarks, gathered using [wrk](https://github.com/wg/wrk) in my
[stress-test.sh](https://github.com/averagewagon/microsite/blob/main/scripts/stress-test.sh)
script:

| **Metric**            | **Description**                                                   | **Value**    |
| --------------------- | ----------------------------------------------------------------- | ------------ |
| **Throughput**        | The number of requests the site can process per second.           | n requests/s |
| **Average Load Time** | The average time it takes for the website to fully load.          | n seconds    |
| **Dropped Requests**  | The percentage or number of requests that fail to be processed.   | n%           |
| **Response Time**     | The time taken by the server to respond to a request.             | n ms         |
| **Uptime**            | The percentage of time the website is accessible to users.        | n%           |
| **Error Rate**        | The percentage of requests that result in errors.                 | n%           |
| **Latency**           | The delay before the transfer of data begins following a request. | n ms         |

If you're interested in more information about how I implemented this, read on.

## ESP-IDF's built-in HTTP server

Since I'm using an ESP32-S3 as my main compute unit for the Microsite, I get
access to the ESP-IDF software suite. This is a little embedded platform which
exposes many different components you can use. One of these components is a
simple HTTP server, called `esp-httpd`. The kind folks at Espressif also include
a layer which utilizes the TLS library `mbedtls` and upgrades the HTTP server
into HTTPS.

This HTTPS server implements many of the basic components I needed. I didn't
need to worry about TCP session management, keep-alives, or listening on a
socket. Additionally, not needing to worry about actually setting up the TLS
sessions and doing the key exchange is nice.

The way that `esp-httpd` tells me when somebody visits my website is via a
callback. When I initialize `esp-httpd`, it asks me to provide a callback I can
invoke whenever I have an incoming HTTP request. Later on, while the server is
running, if somebody attempts to load a webpage (e.g.
https://joni-on-micro.site), `esp-httpd` will invoke my callback and provide me
with details about the request. Most critically, the callback has a field for
the URI, which is the specific resource being requested from my server. If you
visit https://joni-on-micro.site, then the URI will be `/`. If you visit
https://joni-on-micro.site/resume, then the URI will be `/resume`.

TODO: add a bit like this:

- Basically, the built-in HTTP server was made to serve really simple purposes
- Most commonly, this sort of thing is used so you can input wifi credentials
  into a new device
- The microcontroller creates its own acccess point, you connect to it, you type
  your wifi credentials into a little screen, and then the device uses those to
  connect to your normal access point
- As such, the HTTP server is really quite barebones, just enough so that you
  can serve some hard-coded html, record POST responses, and this one even has a
  websocket
- I'm gonna use it for a little bit more than that

TODO: I don't like mentioning URIs here, maybe move it around

TODO: Localhost?

The `esp-httpd` also provides me with a handle to the request, which allows me
to send a response, which is a simple string and a couple of header fields.

That's it. There's no caching, resource management, or built-in 404 pages. There
isn't even a filesystem - everything from this point on is stuff that I had to
add myself.

## Connecting to the webserver

When I something something MAC address and my router registered it as a device
which was on my local network and gave it a local IP address and then I was able
to go on my laptopor phone and just type that local IP address into the address
bar and boom, I loaded up the website which was served by the httpd. This is
basically how all webservers work, it's just that this one wasn't connected to
the public internet.

This is because stuff is discoverable on the same network, so as long as my
router doesn't do any protections (which it actually did, thanks openwrt),
you're able to poke around on stuff on the same network

## Adding a filesystem

When I started out, I registered a very simple callback, which was hard-coded to
return the same string every time:

```
<h1>Welcome to the Microsite!</h1>
<p>This is a very basic HTML webpage.</p>
```

It didn't matter if you visited the 69.69.69.69, or if you visited
69.69.69.69/resume, or if you visited 69.69.69.69/blog/the-name-of-my-blog-post.
You always received the same page, and my code didn't even care about what page
you had requested.

The first thing I wanted to be able to do was serve multiple different web
pages, differentiating them by URI. This is functionality that is built into
almost every webserver, by default, but of course I'm using a microcontroller
and therefore this has to be built out by me instead.

To serve different files at different webpages, I first need the ability to
represent files at all. For this, I chose to use littlefs, which is a filesystem
designed for low-resource microcontrollers such as the ESP32-S3. It features all
the basics

- what
- even
- are
- the
- basics
- of
- ltitlefs

I picked littlefs because it seemed to just be the filesystem with the best
support on the ESP32 - I've heard of it around, like on the zephyr project, and
while I briefly considered FAT32, LittleFS just had more stuff come up on search
engines.

I added the LittleFS module from (TODO: insert author here), which pretty much
worked out-of-the-box in ESP-IDF. I modified my partition table to add a section
in the flash storage which would serve as the LittleFS partition, and since I
have 16MB of flash storage, I pretty much just made 15.5MB of it into my
littleFS partition.

Of course, at that point, I had a way of representing files on my ESP32-S3. My
web server was still just serving that hard-coded HTML, so I had to connect
them. I made a really basic wildcard-based handler, which at first just took
whatever resource was requested by the user and hooked it up directly to a file
of that name. This pretty much worked, although over time I added more polish
and support for basic webserver behaviors people take for granted, like

- If you visit the root of a site (or the root of any folder), you're supposed
  to serve up index.html for that folder
- I added a cache in front of the flash memory which just loaded as much as it
  could into PSRAM, because turns out it's actually quite slow to access flash
  memory, especially external flash memory.
- Some basic serving logic where I twiddle with HTTP headers and serve the
  brotli-compressed version of the HTML file instead of the raw HTML file, if
  the visitor's browser indicates it supports that sort of thing. My flashing
  script pre-compresses the web pages and loads them into flash, so no
  compression is done on the ESP32-S3 itself.

Lastly, I wrote up a quick script which takes an input directory, creates a
LittleFS image which represents that directory, and then saves it into the flash
storage of the ESP32-S3 at the appropriate offset. This gave me the ability to
actually test what I'd done, and have a simple interface by which I could put
things onto the microcontroller.

Now, that gave me a way to serve multiple pages by having multiple files, and
then load them from flash in response to visitors. There's something pretty
important missing here: what is the actual website that I'm going to serve?

## Designing a website for a microcontroller

I immediately turned to static site generators, because I find them to be the
most simple and fun way for me to make hobby websites. At first, I wrote my own
in shell script, basically creating little template strings and then doing
substitutions on them

TODO: Example of my template string on my hand-made SSG

This was pretty fun, but quickly I got bored of it. I didn't want to make a
static site generator, I wanted to make a microcontroller host my blog.

After poking around at options for a while, I settled on Hugo. Hugo is fairly
straightforward, and I liked the way it let you apply multiple substititions in
a row to a single template string, using its daisy chaining stuff

TODO: Use the actual verbiage that Hugo uses

During my initial performance testing of my website, I realized that the
performance bottleneck was setting up a new session for each resource the client
requests. Loading external resources like css, favicon, and images each needed
to set up their own HTTPS session, and it was brutally slow. The ESP-IDF http
server also only supports HTTP 1.1, so I didn't have the option of reusing a
single HTTPS session for multiple resources (multiplexing), which didn't come
along until HTTP 2.

To get around this, I made a couple of Hugo shortcodes (basically macros) which
would embed all resource files directly in the HTML page that I was generating.
For images, I'd base64 encode the content, and then place it on the page as a
data:uri(TODO: verbiage) sourced-image. These technically inflated the sizes of
everything, and also means I can't cache CSS or image files reused across
different pages, but but between serving compressed web pages and not needing to
set up new HTTPS sessions, I saw an enormous dramatic speedup in web
performance.

I then did the thing I'd been avoiding the entire time: actually designing a
website.

I kind of hate web design. Not because I think it's not legitimate as an art
form and whatnot, but because I get sucked into the rabbit hole of minor CSS
tweaks for 12 hours at a time, and then end up with something I dislike more
than when I started.

To make the style for my blog, I first adapted the xmin Hugo template by TODO:
insert author, flipped it into dark mode, and then proceeded to play code golf.
My goal was to have the fewest possible CSS rules which still resulted in a
website I found visually tolerable. This was somewhat because I wanted to have
my CSS be the minimum possible size, for microcontroller performance reasons,
but mostly because I didn't want to spend weeks making a custom pretty website.

Eventually, I arrived at something satisfying enough that I could cut myself
off. I used Mistral to generate some placeholder content by pasting all my
Microsite notes into it, did some minor typesetting, and then called the web
design done. I truly, sincerely hope that I never have to touch it again.

Now, it was time to publish and get this onto the public internet.

## Going Public

To allow people on the public internet to visit my website, I need to expose the
Microsite through my public IP on my router. Then, because I want people to
visit [https://joni-on-micro.site](joni-on-micro.site) and not `136.27.23.63` or
whatever, I need to pay for a domain and register my public IP address with a
DNS registrar. Since the modern web allegedly cares about security, I also
figured I should use the built-in HTTPS features of ESP-IDF's `httpd` and get a
certificate with LetsEncrypt.

My router is an OpenWRT One, so thanks to the power of open source, I get to
take advantage of public documentation:

TODO: Picture of this really sparse web page on OpenWRT port forwarding:
https://openwrt.org/docs/guide-user/firewall/fw3_configurations/port_forwarding

With some documentation reading, finagling, and enjoyable forways into what a
loopback is and such (TODO: find out verbiage for when I'm not allowed to
connect to something publicly when im on the same network), I was able to use
the public IPv4 address supplied by my ISP to connect to my website, from any
other computer on the public network. My webserver was technically public!

This is where the opsec people in the crowd will roast me over the coals - I
don't have a DMZ because I'm kind of stupid. For other, similarly-uninformed
folks, a DMZ is a common strategy for folks who host their own websites where
they have a network for their publicly-accessible resources which is entirely
separate from their regular personal network, where they connect their laptops
and toasters and lightbulbs. I don't have one of these - my website is served
from the same network I pay my taxes through. My blind, naive hope is that
because my only publicly-exposed server is an ESP32-S3, running my dumb little
firmware, I'm insulated from the majority of common webserver attacks that
people would do. If somebody bakes up a personalized attack which overwrites the
flash on my ESP32-S3, gets it running some custom firmware, and opens up an
attack on my network, I think I'd actually feel quite honored to relinquish my
life savings, social security, and poorly-encrypted personal photos. If anyone
reading this plans to replicate this project, I'd urge against selling any drugs
or planning assassinations with this setup.

With my risk of getting red-teamed aside, it was time to get a domain for my
website. While having folks type `136.27.23.63` directly into their browser
search bar has a sort of hacker appeal, I figured that a human-readable domain
is more memorable. After emailing every publicly-registered employee of an event
hosting company to see if I could purchase `micro.site`, I settled on the
`joni-on-micro.site` domain, which I registered with Namecheap. I added an A
Record which pointed to the public IP address on my router, waited a couple
minutes, and boom - typing [joni-on-micro.site](https://joni-on-micro.site)
loaded up my website.

There was just one lingering issue:

TODO: Picture of the insecure website page, the lock-with-a-cross-through-it

My website was unencrypted, served over raw HTTP instead of HTTPS. In theory, I
don't really have a problem with this - I generally don't care if someone
man-in-the-middles my blog, even if something annoying happens, like
[your ISP injecting ads directly into the website](https://datatracker.ietf.org/doc/html/rfc6108).
However, that lock with a strike through it really makes me feel insecure, so I
figured I'd upgrade my site to use HTTPS.

Thankfully, [LetsEncrypt](https://letsencrypt.org/) offers free TLS
certificates. I used the `certbot` utility to generate an ECDSA cert for the
Microsite, using the `secp256r1` elliptic curve because it's well-supported and
results in smaller certificate sizes than RSA. I placed both the certificate and
the private key into regular unencrypted flash on the ESP32-S3 (sorry, opsec
folks), configured ESP-IDF's `httpd` to force all HTTP connections to upgrade to
HTTPS, tweaked my DNS settings on Namecheap, was rewarded with that glorious
symbol:

TODO: Picture of the lock icon

With this, [joni-on-micro.site](https://joni-on-micro.site) was fully online,
serving some webpages over HTTPS from my studio apartment.

## Wrapping Up

I did the vast majority of the technical work for Microsite project in Jan-Feb
of 2025, and yet the most difficult part has been writing this article about it.
I made the Microsite for many reasons, but chief among them was to give me a fun
and tweakable platform for me to practice blogging. My hope is that this blog
becomes the most contentful microcontroller-hosted website in the galaxy.

I have a long list of improvements I'd like to make to the Microsite, such as

- Switching to an ethernet-based connection, as WiFi is finnicky and slow
- Making a cluster using multiple Microsite boards to scale up the Microsite's
  traffic handling abilities
- Designing a custom PCB that fits in the form factor of a 3.5" HDD, so I can
  make mini server racks

However, for now, I'm quite satisfied to work on my other projects and watch the
Microsite's board dutifully blink behind me.

TODO: Picture of the Microsite on my UPS
