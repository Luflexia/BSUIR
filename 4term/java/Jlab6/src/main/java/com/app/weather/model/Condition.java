package com.app.weather.model;

import jakarta.persistence.*;
import lombok.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Table(name = "condition")
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class Condition {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name = "text")
    private String text;

    // Двунаправленная связь One-to-Many
    @OneToMany(mappedBy = "condition", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Weather> weathers = new ArrayList<>();

    public void addWeather(Weather weather) {
        weathers.add(weather);
        weather.setCondition(this);
    }

}
