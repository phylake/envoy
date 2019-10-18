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

## v1.11.2-2.6.0-adobe

- added `/opt/jaeger/jaeger-tracer.so` to the Docker image

## v1.11.2-2.5.0-adobe

- upgraded envoy

## v1.11.1-2.5.0-adobe

- revert v1.11.1-2.4.0-adobe for a better solution in Contour
- cap upstream idle_timeout to 5 mins

## v1.11.1-2.4.0-adobe

- O(n) config memory growth for SNI

## v1.11.1-2.3.0-adobe

- upgraded envoy

## v1.10.0-2.3.0-adobe

- added `envoy.filters.http.health_check_simple`

## v1.10.0-2.2.0-adobe

- upgraded envoy

## v1.9.1-2.2.0-adobe

- set default upstream idle timeout of 58s if none is provided
- add curl to image for easier `preStop` handling

## v1.9.1-2.1.0-adobe

- increase hardcoded max header size to 64KiB

## v1.9.1-2.0.0-adobe

- upgraded envoy

## v1.9.0-2.0.0-adobe

- upgraded envoy

## v1.8.0-2.0.0-adobe

- breaking change: renamed `envoy.filters.listener.ip` to `envoy.filters.listener.ip_allow_deny`
- added L7 version of IP allow/deny filter. it returns 403s instead of closing the socket

## v1.8.0-1.0.1-adobe

- fixed erroneous `std::make_shared<T>`

## v1.8.0-1.0.0-adobe

- max header size filter
- ip allow/deny listener filter
