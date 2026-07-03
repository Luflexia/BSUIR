package com.app.weather.controller;

import com.app.weather.dto.ConditionDTO;
import com.app.weather.model.Condition;
import com.app.weather.service.ConditionService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/conditions")
public class ConditionController {

    private final ConditionService conditionService;

    public ConditionController(ConditionService conditionService) {
        this.conditionService = conditionService;
    }

    @GetMapping
    public ResponseEntity<List<ConditionDTO>> getAllConditions() {
        List<Condition> conditions = conditionService.getAllConditions();
        List<ConditionDTO> conditionDTOs = conditions.stream()
                .map(conditionService::convertToDTO)
                .toList();
        return ResponseEntity.ok(conditionDTOs);
    }

    @GetMapping("/{id}")
    public ResponseEntity<ConditionDTO> getConditionById(@PathVariable Long id) {
        Condition condition = conditionService.getConditionById(id);
        if (condition == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(conditionService.convertToDTO(condition));
    }

    @PostMapping
    public ResponseEntity<ConditionDTO> createCondition(@RequestBody ConditionDTO conditionDTO) {
        Condition condition = conditionService.convertToEntity(conditionDTO);
        Condition savedCondition = conditionService.createCondition(condition);
        return ResponseEntity.ok(conditionService.convertToDTO(savedCondition));
    }

    @PutMapping("/{id}")
    public ResponseEntity<ConditionDTO> updateCondition(@PathVariable Long id, @RequestBody ConditionDTO conditionDTO) {
        Condition updatedCondition = conditionService.updateCondition(id, conditionDTO);
        if (updatedCondition == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(conditionService.convertToDTO(updatedCondition));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteCondition(@PathVariable Long id) {
        if (!conditionService.deleteCondition(id)) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.noContent().build();
    }
}
