package com.app.weather.api;

import com.squareup.okhttp.Call;
import com.squareup.okhttp.OkHttpClient;
import com.squareup.okhttp.Request;
import com.squareup.okhttp.Response;

import java.io.IOException;

public class Api {
    private static final String OPEN_WEATHER_BASE_URL = "http://api.openweathermap.org/data/2.5/weather?q=";
    private static final String OPEN_WEATHER_KEY = "&units=metric&APPID=97c7ba9aebec27a04bfdec4cff331b0f";

    private static String getOpenWeatherBaseUrl(String city) {
        return OPEN_WEATHER_BASE_URL + city + OPEN_WEATHER_KEY;
    }

    private static String makeResponse(String url) throws IOException {
        OkHttpClient client = new OkHttpClient();
        Request request = new Request.Builder().url(url).build();
        Call call = client.newCall(request);
        Response response = call.execute();
        return response.body().string();
    }

    public static String getWeatherJson(String city) throws IOException {
        String serviceUrl = getOpenWeatherBaseUrl(city);
        return makeResponse(serviceUrl);
    }

    public static String getOpenWeatherJson(String city) throws IOException {
        return makeResponse(getOpenWeatherBaseUrl(city));
    }
}
