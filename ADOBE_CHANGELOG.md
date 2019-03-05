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

## v1.8.0-2.0.0-adobe

- breaking change: renamed `envoy.filters.listener.ip` to `envoy.filters.listener.ip_allow_deny`
- added L7 version of IP allow/deny filter. it returns 403s instead of closing the socket

## v1.8.0-1.0.1-adobe

- fixed erroneous `std::make_shared<T>`

## v1.8.0-1.0.0-adobe

- max header size filter
- ip allow/deny listener filter
