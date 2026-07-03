package com.app.weather.dto;

public class WeatherDTO {
    private Long id;
    private String city;
    private String weatherData;

    public WeatherDTO(Long id, String city, String weatherData) {
        this.id = id;
        this.city = city;
        this.weatherData = weatherData;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getCity() {
        return city;
    }

    public void setCity(String city) {
        this.city = city;
    }

    public String getWeatherData() {
        return weatherData;
    }

    public void setWeatherData(String weatherData) {
        this.weatherData = weatherData;
    }
}