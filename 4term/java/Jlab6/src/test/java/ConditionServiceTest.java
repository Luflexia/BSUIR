import com.app.weather.component.CacheComponent;
import com.app.weather.component.CustomLogger;
import com.app.weather.dto.ConditionDTO;
import com.app.weather.exceptions.BadRequestException;
import com.app.weather.exceptions.InternalServerErrorException;
import com.app.weather.model.Condition;
import com.app.weather.repository.ConditionRepository;
import com.app.weather.service.ConditionService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class ConditionServiceTest {

    @Mock
    private ConditionRepository conditionRepository;
    @Mock
    private CacheComponent cache;
    @Mock
    private CustomLogger customLogger;
    private ConditionService conditionService;

    @BeforeEach
    void setup() {
        MockitoAnnotations.initMocks(this);
        conditionService = new ConditionService(conditionRepository, cache, customLogger);
    }

    @Test
    void testCreateCondition() {
        Condition condition = new Condition();
        condition.setId(1L);
        condition.setText("Sunny");
        when(conditionRepository.save(condition)).thenReturn(condition);

        Condition createdCondition = conditionService.createCondition(condition);

        assertEquals(condition, createdCondition);
        verify(conditionRepository, times(1)).save(condition);
        verify(cache, times(1)).put(condition.getId().toString(), createdCondition);
    }

    @Test
    void testCreateConditionWithExistingTextThrowsBadRequestException() {
        Condition condition = new Condition();
        condition.setText("Sunny");

        when(conditionRepository.existsByText(condition.getText())).thenReturn(true);

        assertThrows(BadRequestException.class, () -> conditionService.createCondition(condition));
        verify(conditionRepository, never()).save(condition);
        verify(cache, never()).put(anyString(), any(Condition.class));
    }

    @Test
    void testUpdateCondition() {
        Condition existingCondition = new Condition();
        existingCondition.setId(1L);
        existingCondition.setText("Sunny");

        ConditionDTO conditionDTO = new ConditionDTO();
        conditionDTO.setText("Cloudy");

        when(conditionRepository.findById(existingCondition.getId())).thenReturn(Optional.of(existingCondition));
        when(conditionRepository.save(existingCondition)).thenReturn(existingCondition);

        Condition updatedCondition = conditionService.updateCondition(existingCondition.getId(), conditionDTO);

        assertEquals(conditionDTO.getText(), updatedCondition.getText());
        verify(conditionRepository, times(1)).save(existingCondition);
    }

    @Test
    void testUpdateConditionWithNonExistingIdThrowsBadRequestException() {
        ConditionDTO conditionDTO = new ConditionDTO();
        conditionDTO.setText("Cloudy");

        when(conditionRepository.findById(1L)).thenReturn(Optional.empty());

        assertThrows(InternalServerErrorException.class, () -> conditionService.updateCondition(1L, conditionDTO));
        verify(conditionRepository, never()).save(any(Condition.class));
        verify(cache, never()).put(anyString(), any(Condition.class));
    }

    @Test
    void testDeleteCondition() {
        Condition condition = new Condition();
        condition.setId(1L);
        condition.setText("Test condition");
        when(conditionRepository.save(condition)).thenReturn(condition);
        when(conditionRepository.findById(condition.getId())).thenReturn(Optional.of(condition));
        when(conditionRepository.existsById(condition.getId())).thenReturn(true);

        conditionService.createCondition(condition);

        Long id = condition.getId();

        conditionService.deleteCondition(id);

        verify(conditionRepository, times(1)).deleteById(id);
        verify(cache, times(1)).remove(id.toString());
    }

    @Test
    void testGetConditionById() {
        Condition condition = new Condition();
        condition.setId(1L);
        condition.setText("Sunny");

        when(conditionRepository.findById(condition.getId())).thenReturn(Optional.of(condition));

        Condition foundCondition = conditionService.getConditionById(condition.getId());

        assertEquals(condition, foundCondition);
        verify(conditionRepository, times(1)).findById(condition.getId());
        verify(cache, times(1)).put(condition.getId().toString(), foundCondition);
    }

    @Test
    void testGetConditionByIdWithNonExistingIdThrowsBadRequestException() {
        Long id = 1L;

        when(conditionRepository.findById(id)).thenReturn(Optional.empty());

        assertThrows(InternalServerErrorException.class, () -> conditionService.getConditionById(id));
        verify(cache, never()).put(anyString(), any(Condition.class));
    }

    @Test
    void testGetAllConditions() {
        List<Condition> conditions = new ArrayList<>();
        conditions.add(new Condition());
        conditions.add(new Condition());

        when(conditionRepository.findAll()).thenReturn(conditions);

        assertThrows(InternalServerErrorException.class, () -> conditionService.getAllConditions());

        verify(conditionRepository, times(1)).findAll();
    }

    @Test
    void testGetConditionByText() {
        String text = "Sunny";
        Condition condition = new Condition();
        condition.setId(1L); // устанавливаем идентификатор
        condition.setText(text);

        when(conditionRepository.findByText(text)).thenReturn(condition);

        Condition foundCondition = conditionService.getConditionByText(text);

        assertEquals(condition, foundCondition);
        verify(conditionRepository, times(1)).findByText(text);
        verify(cache, times(1)).put(foundCondition.getId().toString(), foundCondition);
    }

    @Test
    void testGetConditionByTextWithNonExistingTextReturnsNull() {
        String text = "Sunny";
        when(conditionRepository.findByText(text)).thenReturn(null);
        Condition condition = conditionService.getConditionByText(text);
        assertNull(condition);
        verify(conditionRepository, times(1)).findByText(text);
        verify(cache, never()).put(anyString(), any(Condition.class));
    }

    @Test
    void testCreateConditionBulk() {
        List<ConditionDTO> conditionDTOs = new ArrayList<>();
        conditionDTOs.add(new ConditionDTO(null, "Sunny"));
        conditionDTOs.add(new ConditionDTO(null, "Cloudy"));
        conditionDTOs.add(new ConditionDTO(null, "Rainy"));

        when(conditionRepository.existsByText(anyString())).thenReturn(false);
        when(conditionRepository.save(any(Condition.class))).thenAnswer(invocation -> invocation.getArgument(0));

        List<Condition> createdConditions = conditionService.createConditionBulk(conditionDTOs);

        assertEquals(3, createdConditions.size());
        verify(conditionRepository, times(3)).save(any(Condition.class));
        verify(cache, never()).put(anyString(), any(Condition.class));
    }

    @Test
    void testUpdateNonExistingConditionThrowsBadRequestException() {
        ConditionDTO conditionDTO = new ConditionDTO(null, "Cloudy");

        when(conditionRepository.findById(1L)).thenReturn(Optional.empty());

        assertThrows(InternalServerErrorException.class, () -> conditionService.updateCondition(1L, conditionDTO));
        verify(conditionRepository, never()).save(any(Condition.class));
        verify(cache, never()).put(anyString(), any(Condition.class));
    }

    @Test
    void testUpdateConditionWithExistingValueThrowsBadRequestException() {
        Condition existingCondition = new Condition();
        existingCondition.setId(1L);
        existingCondition.setText("Sunny");

        ConditionDTO conditionDTO = new ConditionDTO(null, "Cloudy");

        when(conditionRepository.findById(existingCondition.getId())).thenReturn(Optional.of(existingCondition));
        when(conditionRepository.existsByTextAndIdNot("Cloudy", existingCondition.getId())).thenReturn(true);

        assertThrows(BadRequestException.class, () -> conditionService.updateCondition(1L, conditionDTO));

        verify(conditionRepository, never()).save(any(Condition.class));
    }

    @Test
    void testDeleteNonExistingConditionThrowsBadRequestException() {
        when(conditionRepository.existsById(1L)).thenReturn(false);

        assertThrows(BadRequestException.class, () -> conditionService.deleteCondition(1L));
        verify(conditionRepository, never()).deleteById(anyLong());
        verify(cache, never()).remove(anyString());
    }

    @Test
    void testGetConditionByTextWithNonExistingTextThrowsInternalServerErrorException() {
        String text = "Sunny";
        when(conditionRepository.findByText(text)).thenThrow(new RuntimeException());

        assertThrows(InternalServerErrorException.class, () -> conditionService.getConditionByText(text));
        verify(conditionRepository, times(1)).findByText(text);
        verify(cache, never()).put(anyString(), any(Condition.class));
    }


}
