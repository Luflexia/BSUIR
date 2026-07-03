package com.app.weather.controller;

import com.app.weather.dto.WeatherDTO;
import com.app.weather.model.Weather;
import com.app.weather.service.WeatherService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/weather")
public class WeatherController {

    private final WeatherService weatherService;

    public WeatherController(WeatherService weatherService) {
        this.weatherService = weatherService;
    }

    @PostMapping
    public ResponseEntity<WeatherDTO> createWeatherWithConditionText(@RequestBody WeatherDTO weatherDTO) {
        Weather createdWeather = weatherService.createWeatherWithCondition(weatherDTO);
        return ResponseEntity.ok(weatherService.convertToDTO(createdWeather));
    }

    @GetMapping("/{id}")
    public ResponseEntity<WeatherDTO> getWeatherById(@PathVariable Long id) {
        Weather weather = weatherService.getWeatherById(id);
        if (weather == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(weatherService.convertToDTO(weather));
    }

    @PutMapping("/{id}")
    public ResponseEntity<WeatherDTO> updateWeather(@PathVariable Long id, @RequestBody WeatherDTO weatherDTO) {
        Weather weather = weatherService.updateWeather(id, weatherDTO);
        if (weather == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(weatherService.convertToDTO(weather));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteWeather(@PathVariable Long id) {
        weatherService.deleteWeather(id);
        return ResponseEntity.noContent().build();
    }

    @GetMapping
    public ResponseEntity<List<WeatherDTO>> getAllWeathers() {
        List<Weather> weathers = weatherService.getAllWeathers();
        List<WeatherDTO> weatherDTOs = weathers.stream()
                .map(weatherService::convertToDTO)
                .toList();
        return ResponseEntity.ok(weatherDTOs);
    }
}
