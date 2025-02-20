package com.app.weather.controller;

import com.app.weather.model.Weather;
import com.app.weather.service.WeatherService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api")
public class WeatherController {
    private final WeatherService weatherService;

    public WeatherController(WeatherService weatherService) {
        this.weatherService = weatherService;
    }

    @GetMapping("/weather")
    public ResponseEntity<Weather> getWeather(@RequestParam(value = "city") String city) {
        Weather weather = weatherService.getWeather(city);
        if (weather != null) {
            return ResponseEntity.ok().body(weather);
        } else {
            return ResponseEntity.notFound().build();
        }
    }
}
