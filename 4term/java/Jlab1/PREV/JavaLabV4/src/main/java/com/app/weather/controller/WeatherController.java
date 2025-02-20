package com.app.weather.controller;

import com.app.weather.model.Weather;
import com.app.weather.repository.WeatherRepository;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

import static com.app.weather.api.Api.*;

@RestController
@RequestMapping("/api")
@Controller
public class WeatherController {

    @Autowired
    WeatherRepository weatherRepository;
    /*
     *   Weather information from the OPENWEATHER service for the required city.
     *   Without writing to the database
     */
    @GetMapping("/openWeather/{city}")
    public JSONObject getOpenWeather(@PathVariable final String city) throws IOException, ParseException {
        JSONParser parser = new JSONParser();
        return (JSONObject) parser.parse(getOpenWeatherJson(city));
    }
    /*
     *   Weather information on the required city.
     *   Recording and uploading from a database
     */
    @GetMapping("/{city}")
    public JSONObject getWeatherFromCityName(@PathVariable final String city) throws IOException, ParseException {
        JSONParser parser = new JSONParser();
        saveWeatherToDB(city);
        String weather = weatherRepository.findFirstByOrderByCreatedAtDesc().getOpenWeatherData();
        return (JSONObject) parser.parse(weather);
    }

    private void saveWeatherToDB(String city) throws IOException {
        Weather weather = new Weather(city, getOpenWeatherJson(city));
        weatherRepository.save(weather);
    }
}
