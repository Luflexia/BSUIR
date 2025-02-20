package com.app.weather.api;

public enum NameServices {
    OPEN_WEATHER("openWeather");

    private final String serviceName;

    NameServices(String service){
        this.serviceName = service;
    }

    public String getServiceName(){
        return this.serviceName;
    }


}
