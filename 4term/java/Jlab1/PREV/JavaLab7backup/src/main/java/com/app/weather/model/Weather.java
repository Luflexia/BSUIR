package com.app.weather.model;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import org.springframework.data.annotation.CreatedDate;

import java.sql.Timestamp;
import java.time.LocalDate;


@Entity
@Table(name="weather")
@JsonIgnoreProperties(value = {"createdAt"}, allowGetters = true)
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class Weather {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name="date")
    private Timestamp date;

    @Column(name="city")
    private String city;

    @Column(name="weather_data")
    private String weatherData;
}
