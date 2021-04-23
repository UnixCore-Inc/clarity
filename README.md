# Clarity
quando il css ti fa schifo e tu hai del tempo libero

```
$var {
  x: 10px,
  displayMode: block;
}

$state gian {
  display: displayMode;
  margin: x;
}

body {
  [gian]
}

.fil {
  color: yellow;
}

.div extends body, .fil {
  background-color: red;
}

```

diventa:

```css
body {
  display:  block;
  margin:  10px;
}

.fil {
  color:  yellow;
}

.div {
  background-color:  red;
  color:  yellow;
  margin:  10px;
  display:  block;
}
```
