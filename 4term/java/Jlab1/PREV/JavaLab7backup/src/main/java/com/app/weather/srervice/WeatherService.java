package com.app.weather.srervice;

import com.app.weather.model.Weather;
import com.app.weather.repository.WeatherRepository;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Service
public class WeatherService {
    private final WeatherRepository weatherRepository;

    public WeatherService(WeatherRepository weatherRepository) {
        this.weatherRepository = weatherRepository;
    }

    public Weather getWeatherById(long id) {
        return weatherRepository.findById(id);
    }

    public Weather getWeatherByCity(String city) {
        return weatherRepository.findByCity(city);
    }
    public List<Weather> getAllWeather() {
        return weatherRepository.findAll();
    }

    public void deleteWeatherById(long id) {
        weatherRepository.deleteById(id);
    }

    public Weather createWeather(Weather weather) {
        String city = weather.getCity();
        Weather existingWeather = weatherRepository.findByCity(city);

        if (existingWeather != null) {
            // Если запись с таким городом уже существует, обновляем ее
            existingWeather.setWeatherData(weather.getWeatherData());
            existingWeather.setDate(Timestamp.valueOf(LocalDateTime.now()));
            return weatherRepository.save(existingWeather);
        } else {
            // Если записи с таким городом нет, создаем новую
            weather.setDate(Timestamp.valueOf(LocalDateTime.now()));
            return weatherRepository.save(weather);
        }
    }
    public Weather getWeather(String city) {
        return weatherRepository.findByCity(city);
    }


}
