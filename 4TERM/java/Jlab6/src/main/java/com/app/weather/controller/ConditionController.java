package com.app.weather.controller;

import com.app.weather.component.CustomLogger;
import com.app.weather.dto.ConditionDTO;
import com.app.weather.model.Condition;
import com.app.weather.service.ConditionService;
import com.app.weather.service.CounterService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@CrossOrigin
@RequestMapping("/conditions")
public class ConditionController {
    private static final String COUNTER_MSG = "\nCounter: ";
    private final ConditionService conditionService;

    private final CounterService counterService;
    private final CustomLogger customLogger;

    public ConditionController(ConditionService conditionService, CustomLogger customLogger, CounterService counterService) {
        this.conditionService = conditionService;
        this.customLogger = customLogger;
        this.counterService = counterService;
    }

    @PostMapping("/bulk")
    public ResponseEntity<List<ConditionDTO>> createConditionBulk(@RequestBody List<ConditionDTO> conditionDTOs) {
        customLogger.info("Creating bulk of conditions" + COUNTER_MSG + counterService.incrementAndGet());
        List<Condition> createdConditions = conditionService.createConditionBulk(conditionDTOs);
        return ResponseEntity.ok(createdConditions.stream()
                .map(conditionService::convertToDTO)
                .toList());
    }

    @GetMapping
    public ResponseEntity<List<ConditionDTO>> getAllConditions() {
        customLogger.info("Получение всех condition" + COUNTER_MSG + counterService.incrementAndGet());
        List<Condition> conditions = conditionService.getAllConditions();
        List<ConditionDTO> conditionDTOs = conditions.stream()
                .map(conditionService::convertToDTO)
                .toList();
        return ResponseEntity.ok(conditionDTOs);
    }

    @GetMapping("/{id}")
    public ResponseEntity<ConditionDTO> getConditionById(@PathVariable Long id) {
        customLogger.info("Получение condition по id: " + id + COUNTER_MSG + counterService.incrementAndGet());
        Condition condition = conditionService.getConditionById(id);
        if (condition == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(conditionService.convertToDTO(condition));
    }

    @PostMapping
    public ResponseEntity<ConditionDTO> createCondition(@RequestBody ConditionDTO conditionDTO) {
        customLogger.info("Создание условия" + COUNTER_MSG + counterService.incrementAndGet());
        Condition condition = conditionService.convertToEntity(conditionDTO);
        Condition savedCondition = conditionService.createCondition(condition);
        return ResponseEntity.ok(conditionService.convertToDTO(savedCondition));
    }

    @PutMapping("/{id}")
    public ResponseEntity<ConditionDTO> updateCondition(@PathVariable Long id, @RequestBody ConditionDTO conditionDTO) {
        customLogger.info("Обновление condition с id: " + id + COUNTER_MSG + counterService.incrementAndGet());
        Condition updatedCondition = conditionService.updateCondition(id, conditionDTO);
        if (updatedCondition == null) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.ok(conditionService.convertToDTO(updatedCondition));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteCondition(@PathVariable Long id) {
        customLogger.info("Удаление condition с id: " + id + COUNTER_MSG + counterService.incrementAndGet());
        if (!conditionService.deleteCondition(id)) {
            return ResponseEntity.notFound().build();
        }
        return ResponseEntity.noContent().build();
    }
}
