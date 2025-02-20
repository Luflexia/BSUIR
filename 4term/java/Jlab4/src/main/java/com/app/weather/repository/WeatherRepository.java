package com.app.weather.repository;
import com.app.weather.model.Weather;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface WeatherRepository extends JpaRepository<Weather, Long> {
    Weather findByCity(String city);

    boolean existsByCity(String city);

    boolean existsByCityAndIdNot(String city, Long id);

    //JPQL
    @Query("SELECT w FROM Weather w WHERE w.temperature = :temperature")
    List<Weather> findByTemperature(@Param("temperature") double temperature);

    @Query("SELECT w FROM Weather w JOIN w.condition c WHERE c.text = :conditionText")
    List<Weather> findByConditionText(@Param("conditionText") String conditionText);
}
