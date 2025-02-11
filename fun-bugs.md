# Fun Bugs

Using this file to keep track of interesting behaviors I encountered during
the  
development of the Microsite.

## Newline Changes Link Color in `<a>` Tag

### **Summary**

Wrapping a Hugo shortcode in an `<a>` tag causes different rendering behavior  
depending on whether the shortcode appears on the same line as the opening
`<a>`  
or on a new line.

- **Same line:** The `<figure>` inside the shortcode is treated as **inline**,  
  causing:
  - The link’s default blue/purple color to override the figure’s styles.
  - The media to inherit `text-decoration`, making it appear underlined.
  - Float and margin behaviors to be inconsistent.

```md
<a href="https://github.com/averagewagon/microsite">{{< media "microsite.svg" "Microsite's GitHub" "true" "" "margin:1em;max-width:20%;min-width:40px;float:right;" >}}</a>
```

- **New line:** The `<figure>` retains its **block behavior**, leading to:
  - The figure's color overriding the link’s default styling.
  - No text-decoration applied.
  - Differences in float positioning.

```md
<a href="https://github.com/averagewagon/microsite">  
  {{< media "microsite.svg" "Microsite's GitHub" "true" "" "margin:1em;max-width:20%;min-width:40px;float:right;" >}}  
</a>
```

This happens because **whitespace affects how browsers parse block vs. inline  
elements**, and minification doesn’t change the parsing behavior.

---

## Espressif WiFi Driver Requires Ethernet Symbols

### **Summary**

The Espressif-provided proprietary WiFi driver for Zephyr depends on Ethernet  
symbols, preventing it from being used in a strictly WiFi-only configuration.

- **Expected behavior:** The WiFi driver should operate independently of
  Ethernet.
- **Actual behavior:** The build fails with undefined references to
  `ethernet_init`,  
  `net_eth_carrier_on`, and `net_eth_carrier_off`, even when Ethernet is
  disabled.

### **Investigation**

- Enabling `CONFIG_NET_L2_ETHERNET` resolves the issue, confirming that the  
  proprietary firmware blob assumes Ethernet support.
- The driver appears to reuse parts of the Ethernet stack for link management,  
  even though it does not require a physical Ethernet connection.
- If Ethernet is explicitly disabled (`CONFIG_NET_L2_ETHERNET=n`), the  
  driver fails to link unless mocks or stub implementations are provided.

### **Implications**

- Any Zephyr project using Espressif’s WiFi propritary firmware blob **must
  enable Ethernet** to satisfy the dependencies, even if Ethernet is not used.
- This increases the binary size and may introduce unnecessary dependencies.
- Workarounds could involve providing stub implementations of the missing
  symbols.
