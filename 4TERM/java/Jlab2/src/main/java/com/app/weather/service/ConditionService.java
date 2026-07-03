package com.app.weather.service;

import com.app.weather.dto.ConditionDTO;
import com.app.weather.model.Condition;
import com.app.weather.repository.ConditionRepository;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class ConditionService {

    private final ConditionRepository conditionRepository;

    public ConditionService(ConditionRepository conditionRepository) {
        this.conditionRepository = conditionRepository;
    }

    public Condition createCondition(Condition condition) {
        Condition existingCondition = conditionRepository.findByText(condition.getText());
        if (existingCondition != null) {
            return existingCondition;
        }
        return conditionRepository.save(condition);
    }

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
        return conditionRepository.save(existingCondition);
    }

    public boolean deleteCondition(Long id) {
        if (!conditionRepository.existsById(id)) {
            return false;
        }
        conditionRepository.deleteById(id);
        return true;
    }

    public Condition getConditionById(Long id) {
        return conditionRepository.findById(id).orElse(null);
    }

    public List<Condition> getAllConditions() {
        return conditionRepository.findAll();
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
    public Condition getConditionByText(String text) {
        return conditionRepository.findByText(text);
    }
}
