# Fun Bugs

Using this file to keep track of interesting behaviors I encountered during the
development of the Microsite.

## Newline Changes Link Color in `<a>` Tag

### **Summary**

Wrapping a Hugo shortcode in an `<a>` tag causes different rendering behavior
depending on whether the shortcode appears on the same line as the opening `<a>`
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
