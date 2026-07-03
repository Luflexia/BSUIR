package com.app.weather.controller;

import com.app.weather.component.CustomLogger;
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
    private final CustomLogger customLogger;

    public WeatherController(WeatherService weatherService, CustomLogger customLogger) {
        this.weatherService = weatherService;
        this.customLogger = customLogger;
    }

    @PostMapping("/bulk")
    public ResponseEntity<List<WeatherDTO>> createWeatherBulk(@RequestBody List<WeatherDTO> weatherDTOs) {
        customLogger.info("Creating bulk of weathers");
        List<Weather> createdWeathers = weatherService.createWeatherBulk(weatherDTOs);
        return ResponseEntity.ok(createdWeathers.stream()
                .map(weatherService::convertToDTO)
                .toList());
    }

    @GetMapping
    public ResponseEntity<List<WeatherDTO>> getAllWeathers() {
        customLogger.info("Получение всех weather");
        List<Weather> weathers = weatherService.getAllWeathers();
        List<WeatherDTO> weatherDTOs = weathers.stream()
                .map(weatherService::convertToDTO)
                .toList();
        return ResponseEntity.ok(weatherDTOs);
    }

    @GetMapping("/{id}")
    public ResponseEntity<WeatherDTO> getWeatherById(@PathVariable Long id) {
        customLogger.info("Получение weather по id: " + id);
        Weather weather = weatherService.getWeatherById(id);
        if (weather == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(weatherService.convertToDTO(weather));
    }

    //Query
    @GetMapping("/citiesT/{temperature}")
    public ResponseEntity<List<WeatherDTO>> getWeatherByTemperature(@PathVariable double temperature) {
        customLogger.info("Получение weather по температуре: " + temperature);
        List<WeatherDTO> weatherDTOs = weatherService.findByTemperature(temperature);
        return ResponseEntity.ok(weatherDTOs);
    }

    @GetMapping("/citiesC/{conditionText}")
    public ResponseEntity<List<WeatherDTO>> findByConditionText(@PathVariable String conditionText) {
        customLogger.info("Получение condition по тексту условия: " + conditionText);
        List<WeatherDTO> weathers = weatherService.findByConditionText(conditionText);
        return ResponseEntity.ok(weathers);
    }

    @PostMapping
    public ResponseEntity<WeatherDTO> createWeatherWithConditionText(@RequestBody WeatherDTO weatherDTO) {
        customLogger.info("Создание weather с condition");
        Weather createdWeather = weatherService.createWeatherWithCondition(weatherDTO);
        return ResponseEntity.ok(weatherService.convertToDTO(createdWeather));
    }

    @PutMapping("/{id}")
    public ResponseEntity<WeatherDTO> updateWeather(@PathVariable Long id, @RequestBody WeatherDTO weatherDTO) {
        customLogger.info("Обновление weather с id: " + id);
        Weather weather = weatherService.updateWeather(id, weatherDTO);
        if (weather == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(weatherService.convertToDTO(weather));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteWeather(@PathVariable Long id) {
        customLogger.info("Удаление weather с id: " + id);
        weatherService.deleteWeather(id);
        return ResponseEntity.noContent().build();
    }

}
