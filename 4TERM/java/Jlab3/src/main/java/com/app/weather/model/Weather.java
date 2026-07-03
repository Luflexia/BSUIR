package com.app.weather.model;

import jakarta.persistence.*;
import lombok.*;
import java.sql.Timestamp;

@Entity
@Table(name = "weather")
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

    @Column(name = "temperature")
    private double temperature;

    // Двунаправленная связь Many-to-One
    @ManyToOne
    @JoinColumn(name = "condition_id")
    private Condition condition;
}
