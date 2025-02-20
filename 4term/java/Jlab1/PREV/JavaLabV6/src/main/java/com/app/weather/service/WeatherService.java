package com.app.weather.service;

import com.app.weather.model.Weather;
import com.app.weather.repository.WeatherRepository;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.List;

@Service
public class WeatherService {
    private final WeatherRepository weatherRepository;

    public WeatherService(WeatherRepository weatherRepository) {
        this.weatherRepository = weatherRepository;
    }

    public Weather getWeatherById(long id) {
        return weatherRepository.findById(id).orElse(null);
    }

    public List<Weather> getAllWeather() {
        return weatherRepository.findAll();
    }

    public void deleteWeatherById(long id) {
        weatherRepository.deleteById(id);
    }

    public void createWeather(Weather weather) {
        weather.setDate(Timestamp.valueOf(LocalDateTime.now()));
        weatherRepository.save(weather);
    }

    public Weather getWeather(String city) {
        return weatherRepository.findByCity(city);
    }
}
