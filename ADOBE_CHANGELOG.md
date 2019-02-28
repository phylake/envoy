# About

We maintain a set of patches on top of upstream envoy. They need to be
semantically versioned as well so we understand, for example, that we're
upgrading a fix version of envoy vs. a fix version of our patches.

The scheme is quite simple. The upstream version is first, followed by a hyphen,
followed by our version:

```
E stand for Envoy
A stands for Adobe's changes

v{E major}.{E minor}.{E fix}-{A major}.{A minor}.{A fix}-adobe
```

# Log

## v1.8.0-1.0.0-adobe

- max header size filter
- ip allow/deny listener filter
