package com.app.weather.dto;

import lombok.*;
import java.sql.Timestamp;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class WeatherDTO {
    private Long id;
    private String city;
    private Timestamp date;
    private double temperature;
    private ConditionDTO condition;

}
