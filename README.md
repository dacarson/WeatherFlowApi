![Github License](https://img.shields.io/github/license/dacarson/WeatherFlowApi) ![Github Release](https://img.shields.io/github/v/release/dacarson/WeatherFlowApi?display_name=tag)

# WeatherFlowApi

## Description
This light weight Arduino library has a class (`WeatherFlowData`) that process WeatherFlow event as defined here:
[WeatherFlow Tempest UDP Reference](https://weatherflow.github.io/Tempest/api/udp.html)
When a class is successfully processed, a user defined callback is called.

`WeatherFlowUdp` inherits from `WeatherFlowData`, and will setup a UDP listener for WeatherFlow data packets being broadcast on the local network, and will feed them to WeatherFlowData for processing.

An independant helper class, `WeatherFlowStrings` takes the enumerated types from WeatherFlowData and returns strings. The strings are stored in PROGMEM, aka it returns F() strings.

## Examples
The examples folder contains these examples:
- ### SerialPrinter
This example prints all the values of the event that caused the callback to be invoked. The example shows how to setup `WeatherFlowUdp` with a callback. It prints all the values of the most recently processed event.

- ### PrometheusPublisher
This example publishes the WeatherFlow data as a [Prometheus](https://prometheus.io) data source. It shows how to access all the data stored by `WeatherFlowData` and iterate through it.

## License

This library is licensed under [MIT License](https://opensource.org/license/mit/)
