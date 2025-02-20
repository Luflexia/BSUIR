package com.app.weather.service;

import com.app.weather.component.CacheComponent;
import com.app.weather.component.CustomLogger;
import com.app.weather.dto.ConditionDTO;
import com.app.weather.dto.WeatherDTO;
import com.app.weather.exceptions.BadRequestException;
import com.app.weather.exceptions.InternalServerErrorException;
import com.app.weather.model.Condition;
import com.app.weather.model.Weather;
import com.app.weather.repository.WeatherRepository;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
public class WeatherService {
    private static final String NOT_FOUND_MSG = "Weather not found";
    private static final String ALR_EXISTS_MSG = "Weather for this city already exists";
    private final WeatherRepository weatherRepository;
    private final ConditionService conditionService;
    private final CacheComponent cache;
    private final CustomLogger customLogger;
    private String cacheKey;

    @Autowired
    public WeatherService(WeatherRepository weatherRepository, ConditionService conditionService, CacheComponent cache, CustomLogger customLogger) {
        this.weatherRepository = weatherRepository;
        this.conditionService = conditionService;
        this.cache = cache;
        this.customLogger = customLogger;
    }

    @Transactional
    public Weather createWeatherWithCondition(WeatherDTO weatherDTO) {
        customLogger.info("Creating weather with condition");
        weatherRepository.findByCity(weatherDTO.getCity());
        if (weatherRepository.existsByCity(weatherDTO.getCity())) {
            throw new InternalServerErrorException(ALR_EXISTS_MSG);
        }

        try {
            Weather weather = convertToEntity(weatherDTO);
            weather.setDate(new Timestamp(System.currentTimeMillis()));

            cacheKey = weather.getCity();

            // Проверяем, существует ли уже погода для этого города
            Weather existingWeather = (Weather) cache.get(cacheKey);
            if (existingWeather != null) {
                return existingWeather;
            }

            // Проверяем, существует ли условие, если нет, то создаем его
            Condition condition = conditionService.getConditionByText(weatherDTO.getCondition().getText());
            if (condition == null) {
                condition = conditionService.convertToEntity(weatherDTO.getCondition());
                condition = conditionService.createCondition(condition); // сохраняем объект Condition в базе данных
            }

            // Устанавливаем связь между погодой и условием
            weather.setCondition(condition);
            condition.addWeather(weather);

            // Создаем новую погоду
            Weather savedWeather = weatherRepository.save(weather);
            cache.put(cacheKey, savedWeather);
            return savedWeather;
        } catch (Exception e) {
            throw new InternalServerErrorException("Failed to create weather with condition");
        }
    }

    @Transactional
    public Weather updateWeather(Long id, WeatherDTO weatherDTO) {
        customLogger.info("Updating weather with id: " + id);
        Weather existingWeather = getWeatherById(id);
        if (existingWeather == null) {
            throw new BadRequestException(NOT_FOUND_MSG);
        }
        if (weatherRepository.existsByCityAndIdNot(weatherDTO.getCity(), id)) {
            throw new BadRequestException(ALR_EXISTS_MSG);
        }
        try {
            existingWeather.setDate(new Timestamp(System.currentTimeMillis()));
            existingWeather.setTemperature(weatherDTO.getTemperature());
            cacheKey = existingWeather.getCity();

            // Проверяем, существует ли уже погода для этого города
            Weather weatherByCity = weatherRepository.findByCity(weatherDTO.getCity());
            if (weatherByCity != null && !weatherByCity.getId().equals(id)) {
                return weatherByCity;
            }

            // Проверяем, существует ли условие, если нет, то создаем его
            Condition condition = conditionService.getConditionByText(weatherDTO.getCondition().getText());
            if (condition == null) {
                condition = conditionService.convertToEntity(weatherDTO.getCondition());
            }
            existingWeather.setCondition(condition);

            Weather savedWeather = weatherRepository.save(existingWeather);
            cache.put(cacheKey, savedWeather);
            return savedWeather;
        } catch (Exception e) {
            throw new InternalServerErrorException("Failed to update weather");
        }
    }

    @Transactional
    public List<Weather> createWeatherBulk(List<WeatherDTO> weatherDTOs) {
        List<Weather> createdWeathers = new ArrayList<>();

        for (WeatherDTO weatherDTO : weatherDTOs) {
            if (weatherRepository.existsByCity(weatherDTO.getCity())) {
                throw new BadRequestException(ALR_EXISTS_MSG);
            }

            Weather weather = convertToEntity(weatherDTO);
            weather.setDate(new Timestamp(System.currentTimeMillis()));

            Condition condition = conditionService.getConditionByText(weatherDTO.getCondition().getText());

            weather.setCondition(condition);
            condition.addWeather(weather);

            createdWeathers.add(weatherRepository.save(weather));
        }

        return createdWeathers;
    }

    @Transactional
    public void deleteWeather(Long id) {
        customLogger.info("Deleting weather with id: {}" + id);
        Weather weather = getWeatherById(id);
        if (weather == null) {
            throw new BadRequestException(NOT_FOUND_MSG);
        }
        weatherRepository.delete(weather);
        cacheKey = weather.getCity();
        cache.remove(cacheKey);
    }

    public Weather getWeatherById(Long id) {
        customLogger.info("Getting weather by id: " + id);
        try {
            Weather weather = weatherRepository.findById(id).orElse(null);
            if (weather == null) {
                throw new BadRequestException(NOT_FOUND_MSG);
            }
            cacheKey = weather.getCity();
            cache.put(cacheKey, weather);
            return weather;
        } catch (Exception e) {
            throw new InternalServerErrorException("Weather with this ID doesnt exists");
        }
    }

    @Transactional
    public List<Weather> getAllWeathers() {
        customLogger.info("Getting all weathers");
        try {
            List<Weather> weathers = weatherRepository.findAll();
            weathers.forEach(weather -> {
                cacheKey = weather.getCity();
                cache.put(cacheKey, weather);
            });
            return weathers;
        } catch (Exception e) {
            throw new InternalServerErrorException("Failed to get all weathers");
        }
    }

    public WeatherDTO convertToDTO(Weather weather) {
        customLogger.info("Converting Weather to WeatherDTO");
        WeatherDTO dto = new WeatherDTO();
        dto.setId(weather.getId());
        dto.setCity(weather.getCity());
        dto.setDate(weather.getDate());
        dto.setTemperature(weather.getTemperature());

        ConditionDTO conditionDTO = conditionService.convertToDTO(weather.getCondition());
        dto.setCondition(conditionDTO);
        return dto;
    }

    private Weather convertToEntity(WeatherDTO weatherDTO) {
        customLogger.info("Converting WeatherDTO to Weather");
        Weather weather = new Weather();
        weather.setCity(weatherDTO.getCity());
        weather.setDate(weatherDTO.getDate());
        weather.setTemperature(weatherDTO.getTemperature());
        // Создаем объект Condition на основе conditionText
        Condition condition = new Condition();
        condition.setText(weatherDTO.getCondition().getText());
        weather.setCondition(condition);
        return weather;
    }

    public List<WeatherDTO> findByTemperature(double temperature) {
        customLogger.info("Getting weathers by temperature: " + temperature);
        try {
            List<Weather> weathers = weatherRepository.findByTemperature(temperature);
            return weathers.stream()
                    .map(this::convertToDTO)
                    .toList();
        } catch (Exception e) {
            throw new InternalServerErrorException("Failed to find weathers by temperature");
        }
    }

    public List<WeatherDTO> findByConditionText(String conditionText) {
        customLogger.info("Getting weathers by condition text: " + conditionText);
        try {
            List<Weather> weathers = weatherRepository.findByConditionText(conditionText);
            return weathers.stream()
                    .map(this::convertToDTO)
                    .toList();
        } catch (Exception e) {
            throw new InternalServerErrorException("Failed to find weathers by condition text");
        }
    }
}
