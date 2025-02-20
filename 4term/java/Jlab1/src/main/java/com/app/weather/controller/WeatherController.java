package com.app.weather.controller;

import com.app.weather.dto.WeatherDTO;
import com.app.weather.model.Weather;
import com.app.weather.service.WeatherService;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/api")
public class WeatherController {
    private final WeatherService weatherService;

    public WeatherController(WeatherService weatherService) {
        this.weatherService = weatherService;
    }

    @GetMapping("/weather")
    public WeatherDTO getWeather(@RequestParam(value = "city") String city) {
        Weather weather = weatherService.getWeather(city);
        return convertToDTO(weather);
    }

    @PostMapping("/weather")
    public WeatherDTO addWeather(@RequestBody WeatherDTO weatherDTO) {
        Weather weather = convertToEntity(weatherDTO);
        Weather createdWeather = weatherService.createWeather(weather);
        return convertToDTO(createdWeather);
    }

    @GetMapping("/weather/all")
    public List<WeatherDTO> getAllWeather() {
        List<Weather> allWeather = weatherService.getAllWeather();
        return allWeather.stream()
                .map(this::convertToDTO)
                .toList();
    }

    private WeatherDTO convertToDTO(Weather weather) {
        return new WeatherDTO(weather.getId(), weather.getCity(), weather.getWeatherData());
    }

    private Weather convertToEntity(WeatherDTO weatherDTO) {
        Weather weather = new Weather();
        weather.setId(weatherDTO.getId());
        weather.setCity(weatherDTO.getCity());
        weather.setWeatherData(weatherDTO.getWeatherData());
        return weather;
    }
}