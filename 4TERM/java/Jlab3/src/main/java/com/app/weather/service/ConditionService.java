package com.app.weather.service;

import com.app.weather.component.CacheComponent;
import com.app.weather.dto.ConditionDTO;
import com.app.weather.model.Condition;
import com.app.weather.repository.ConditionRepository;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class ConditionService {

    private final ConditionRepository conditionRepository;
    private final CacheComponent cache;
    private String cacheKey;

    public ConditionService(ConditionRepository conditionRepository, CacheComponent cache) {
        this.conditionRepository = conditionRepository;
        this.cache = cache;
    }

    @Transactional
    public Condition createCondition(Condition condition) {
        Condition existingCondition = conditionRepository.findByText(condition.getText());
        if (existingCondition != null) {
            return existingCondition;
        }
        Condition savedCondition = conditionRepository.save(condition);
        cacheKey = savedCondition.getId().toString();
        cache.put(cacheKey, savedCondition);
        return savedCondition;
    }

    @Transactional
    public Condition updateCondition(Long id, ConditionDTO conditionDTO) {
        Condition existingCondition = getConditionById(id);
        if (existingCondition == null) {
            return null;
        }
        Condition condition = conditionRepository.findByText(conditionDTO.getText());
        if (condition != null && !condition.getId().equals(id)) {
            return condition;
        }
        existingCondition.setText(conditionDTO.getText());
        Condition savedCondition = conditionRepository.save(existingCondition);
        cacheKey = savedCondition.getId().toString();
        cache.put(cacheKey, savedCondition);
        return savedCondition;
    }

    @Transactional
    public boolean deleteCondition(Long id) {
        if (!conditionRepository.existsById(id)) {
            return false;
        }
        conditionRepository.deleteById(id);
        cacheKey = id.toString();
        cache.remove(cacheKey);
        return true;
    }

    @Transactional
    public Condition getConditionById(Long id) {
        cacheKey = id.toString();
        Condition condition = (Condition) cache.get(cacheKey);
        if (condition != null) {
            return condition;
        }
        condition = conditionRepository.findById(id).orElse(null);
        if (condition != null) {
            cache.put(cacheKey, condition);
        }
        return condition;
    }

    @Transactional
    public List<Condition> getAllConditions() {
        List<Condition> conditions = conditionRepository.findAll();
        conditions.forEach(condition -> {
            cacheKey = condition.getId().toString();
            cache.put(cacheKey, condition);
        });
        return conditions;
    }

    public Condition convertToEntity(ConditionDTO conditionDTO) {
        Condition condition = new Condition();
        condition.setText(conditionDTO.getText());
        return condition;
    }

    public ConditionDTO convertToDTO(Condition condition) {
        if (condition == null) {
            return null;
        }
        return new ConditionDTO(condition.getId(), condition.getText());
    }

    @Transactional
    public Condition getConditionByText(String text) {
        Condition condition = conditionRepository.findByText(text);
        if (condition != null) {
            cacheKey = condition.getId().toString();
            cache.put(cacheKey, condition);
        }
        return condition;
    }
}
