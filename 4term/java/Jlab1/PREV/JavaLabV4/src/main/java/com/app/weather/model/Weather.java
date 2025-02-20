package com.app.weather.model;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;
import org.springframework.transaction.annotation.Transactional;
import javax.persistence.*;
import java.io.Serializable;
import java.util.Date;


@Entity
@Table(name="weather")
@EntityListeners(AuditingEntityListener.class)
@JsonIgnoreProperties(value = {"createdAt"}, allowGetters = true)
public class Weather implements Serializable {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, updatable = false)
    @Temporal(TemporalType.TIMESTAMP)
    @CreatedDate
    private Date createdAt;

    private String cityName;

    @Lob
    @Column(name="openWeatherData", columnDefinition="CLOB")
    private String openWeatherData;

    public Weather() {
    }

    public Weather( String cityName, String openWeatherData) {
        this.cityName = cityName;
        this.openWeatherData = openWeatherData;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Date getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(Date createdAt) {
        this.createdAt = createdAt;
    }

    public String getCityName() {
        return cityName;
    }

    public void setCityName(String cityName) {
        this.cityName = cityName;
    }

    @Transactional
    public String getOpenWeatherData() {
        return openWeatherData;
    }

    public void setOpenWeatherData(String openWeatherData) {
        this.openWeatherData = openWeatherData;
    }
}
