package com.app.weather.service;
import com.app.weather.dto.ConditionDTO;
import com.app.weather.dto.WeatherDTO;
import com.app.weather.model.Condition;
import com.app.weather.model.Weather;
import com.app.weather.repository.WeatherRepository;
import org.springframework.stereotype.Service;
import java.sql.Timestamp;
import java.util.List;

@Service
public class WeatherService {

    private final WeatherRepository weatherRepository;
    private final ConditionService conditionService;

    public WeatherService(WeatherRepository weatherRepository, ConditionService conditionService) {
        this.weatherRepository = weatherRepository;
        this.conditionService = conditionService;
    }

    public Weather createWeatherWithCondition(WeatherDTO weatherDTO) {
        Weather weather = convertToEntity(weatherDTO);
        weather.setDate(new Timestamp(System.currentTimeMillis()));

        // Проверяем, существует ли уже погода для этого города
        Weather existingWeather = weatherRepository.findByCity(weather.getCity());
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
        return weatherRepository.save(weather);
    }

    public Weather updateWeather(Long id, WeatherDTO weatherDTO) {
        Weather existingWeather = weatherRepository.findById(id).orElse(null);
        if (existingWeather == null) {
            return null;
        }
        existingWeather.setDate(new Timestamp(System.currentTimeMillis()));
        existingWeather.setTemperature(weatherDTO.getTemperature());

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

        return weatherRepository.save(existingWeather);
    }

    public void deleteWeather(Long id) {
        weatherRepository.deleteById(id);
    }

    public Weather getWeatherById(Long id) {
        return weatherRepository.findById(id).orElse(null);
    }

    public List<Weather> getAllWeathers() {
        return weatherRepository.findAll();
    }

    public WeatherDTO convertToDTO(Weather weather) {
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

}
