package com.app.weather.repository;

import com.app.weather.model.Condition;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface ConditionRepository extends JpaRepository<Condition, Long> {
    Condition findByText(String text);

    boolean existsByText(String text);

    boolean existsByTextAndIdNot(String text, Long id);

}
