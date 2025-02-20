package com.app.weather.model;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.sql.Timestamp;

@Entity
@Table(name = "weather")
@JsonIgnoreProperties(value = {"createdAt"}, allowGetters = true)
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class Weather {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name = "date")
    private Timestamp date;

    @Column(name = "city")
    private String city;

    @Column(name = "weather_data")
    private String weatherData;
}
