package com.app.weather.controller;

import com.app.weather.model.Weather;
import com.app.weather.repository.WeatherRepository;
import com.app.weather.srervice.WeatherService;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.util.List;

//import static com.app.weather.api.Api.*;

@RestController
@RequestMapping("/api")
public class WeatherController {
    private final WeatherService weatherService;

    public WeatherController(WeatherService weatherService) {
        this.weatherService = weatherService;
    }

    @GetMapping("/weather")
    public Weather getWeather(@RequestParam(value = "city") String city) {
        return weatherService.getWeather(city);
    }

    @PostMapping("/weather")
    public Weather addWeather(@RequestBody Weather weather) {
        return weatherService.createWeather(weather);
    }
    @GetMapping("/weather/all")
    public List<Weather> getAllWeather() {
        return weatherService.getAllWeather();
    }

}

