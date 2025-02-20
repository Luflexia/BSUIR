import com.app.weather.dto.ConditionDTO;
import com.app.weather.dto.WeatherDTO;
import com.app.weather.exceptions.BadRequestException;
import com.app.weather.exceptions.InternalServerErrorException;
import com.app.weather.model.Condition;
import com.app.weather.model.Weather;
import com.app.weather.component.CacheComponent;
import com.app.weather.component.CustomLogger;
import com.app.weather.repository.ConditionRepository;
import com.app.weather.repository.WeatherRepository;
import com.app.weather.service.ConditionService;
import com.app.weather.service.WeatherService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Captor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

class WeatherServiceTest {

    @Mock
    private WeatherRepository weatherRepository;
    @Mock
    private ConditionRepository conditionRepository;

    @Mock
    private ConditionService conditionService;

    @Mock
    private CacheComponent cacheComponent;

    @Mock
    private CustomLogger customLogger;

    @Captor
    private ArgumentCaptor<Weather> weatherArgumentCaptor;

    private WeatherService weatherService;

    @BeforeEach
    void setUp() {
        MockitoAnnotations.initMocks(this);
        weatherService = new WeatherService(weatherRepository, conditionService, cacheComponent, customLogger);
    }

    @Test
    void testCreateWeatherBulkSuccess() {
        List<WeatherDTO> weatherDTOs = new ArrayList<>();
        WeatherDTO weatherDTO1 = new WeatherDTO();
        weatherDTO1.setCity("London");
        weatherDTO1.setTemperature(20.0);
        weatherDTO1.setCondition(new ConditionDTO());
        weatherDTO1.getCondition().setText("Cloudy");
        weatherDTOs.add(weatherDTO1);

        WeatherDTO weatherDTO2 = new WeatherDTO();
        weatherDTO2.setCity("Paris");
        weatherDTO2.setTemperature(25.0);
        weatherDTO2.setCondition(new ConditionDTO());
        weatherDTO2.getCondition().setText("Sunny");
        weatherDTOs.add(weatherDTO2);

        Condition condition1 = new Condition();
        condition1.setText("Cloudy");
        Condition condition2 = new Condition();
        condition2.setText("Sunny");

        when(conditionService.getConditionByText("Cloudy")).thenReturn(condition1);
        when(conditionService.getConditionByText("Sunny")).thenReturn(condition2);
        when(weatherRepository.save(any(Weather.class))).thenAnswer(invocation -> invocation.getArgument(0));

        List<Weather> createdWeathers = weatherService.createWeatherBulk(weatherDTOs);

        assertEquals(2, createdWeathers.size());
        verify(conditionService, times(2)).getConditionByText(anyString());
        verify(weatherRepository, times(2)).save(any(Weather.class));
    }

    @Test
    void testCreateWeatherBulkCityAlreadyExists() {
        List<WeatherDTO> weatherDTOs = new ArrayList<>();
        WeatherDTO weatherDTO1 = new WeatherDTO();
        weatherDTO1.setCity("London");
        weatherDTO1.setTemperature(20.0);
        weatherDTO1.setCondition(new ConditionDTO());
        weatherDTO1.getCondition().setText("Cloudy");
        weatherDTOs.add(weatherDTO1);

        WeatherDTO weatherDTO2 = new WeatherDTO();
        weatherDTO2.setCity("London");
        weatherDTO2.setTemperature(25.0);
        weatherDTO2.setCondition(new ConditionDTO());
        weatherDTO2.getCondition().setText("Sunny");
        weatherDTOs.add(weatherDTO2);

        when(weatherRepository.existsByCity("London")).thenReturn(true);

        assertThrows(BadRequestException.class, () -> weatherService.createWeatherBulk(weatherDTOs));
        verify(weatherRepository, times(1)).existsByCity("London");
        verify(conditionService, never()).getConditionByText(anyString());
        verify(weatherRepository, never()).save(any(Weather.class));
    }

    @Test
    void testCreateWeatherBulkTransaction() {
        List<WeatherDTO> weatherDTOs = new ArrayList<>();
        WeatherDTO weatherDTO1 = new WeatherDTO();
        weatherDTO1.setCity("London");
        weatherDTO1.setTemperature(20.0);
        weatherDTO1.setCondition(new ConditionDTO());
        weatherDTO1.getCondition().setText("Cloudy");
        weatherDTOs.add(weatherDTO1);

        WeatherDTO weatherDTO2 = new WeatherDTO();
        weatherDTO2.setCity("Paris");
        weatherDTO2.setTemperature(25.0);
        weatherDTO2.setCondition(new ConditionDTO());
        weatherDTO2.getCondition().setText("Sunny");
        weatherDTOs.add(weatherDTO2);

        Condition condition1 = new Condition();
        condition1.setText("Cloudy");

        when(conditionService.getConditionByText("Cloudy")).thenReturn(condition1);
        when(conditionService.getConditionByText("Sunny")).thenThrow(new RuntimeException());
        when(weatherRepository.save(any(Weather.class))).thenAnswer(invocation -> invocation.getArgument(0));

        assertThrows(RuntimeException.class, () -> weatherService.createWeatherBulk(weatherDTOs));
        verify(conditionService, times(2)).getConditionByText(anyString());
        verify(weatherRepository, times(1)).save(any(Weather.class));
    }

    @Test
    void testCreateWeatherWithConditionWhenConditionExistsShouldCreateWeather() throws BadRequestException, InternalServerErrorException {
        // Подготовка данных
        WeatherDTO weatherDTO = new WeatherDTO();
        weatherDTO.setCity("City");
        weatherDTO.setCondition(new ConditionDTO());
        weatherDTO.getCondition().setText("Condition");

        Condition condition = new Condition();
        condition.setText("Condition");

        Weather weather = new Weather();
        weather.setCity("City");
        weather.setCondition(condition);

        when(conditionService.getConditionByText(weatherDTO.getCondition().getText())).thenReturn(condition);
        when(weatherRepository.save(any(Weather.class))).thenReturn(weather);

        // Вызов метода
        Weather result = weatherService.createWeatherWithCondition(weatherDTO);

        // Проверка результата
        assertEquals(weather, result);
        verify(conditionService, times(1)).getConditionByText(weatherDTO.getCondition().getText());
        verify(weatherRepository, times(1)).save(weatherArgumentCaptor.capture());
        Weather capturedWeather = weatherArgumentCaptor.getValue();
        assertEquals("City", capturedWeather.getCity());
        assertEquals(condition, capturedWeather.getCondition());
    }

    @Test
    void testCreateWeatherWithConditionWhenConditionDoesNotExistShouldThrowBadRequestException() {
        // Подготовка данных
        WeatherDTO weatherDTO = new WeatherDTO();
        weatherDTO.setCity("City");
        weatherDTO.setCondition(new ConditionDTO());
        weatherDTO.getCondition().setText("Condition");

        when(conditionService.getConditionByText("NonExistentCondition")).thenThrow(new BadRequestException("Condition not found"));

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.createWeatherWithCondition(weatherDTO));
        verify(conditionService, times(1)).getConditionByText(weatherDTO.getCondition().getText());
        verify(weatherRepository, never()).save(any(Weather.class));
    }

    @Test
    void testCreateWeatherWithConditionWhenWeatherForCityAlreadyExistsShouldThrowBadRequestException() {
        // Подготовка данных
        WeatherDTO weatherDTO = new WeatherDTO();
        weatherDTO.setCity("City");
        weatherDTO.setCondition(new ConditionDTO());
        weatherDTO.getCondition().setText("Condition");

        Weather existingWeather = new Weather();
        existingWeather.setCity("City");
        existingWeather.setCondition(new Condition());
        existingWeather.getCondition().setText("Condition");

        when(weatherRepository.findByCity(weatherDTO.getCity())).thenReturn(existingWeather);

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.createWeatherWithCondition(weatherDTO));
        verify(weatherRepository, times(1)).findByCity(weatherDTO.getCity());
        verify(weatherRepository, never()).save(any(Weather.class));
    }

    @Test
    void testUpdateWeatherWhenWeatherExistsShouldUpdateWeather() throws BadRequestException, InternalServerErrorException {
        // Подготовка данных
        WeatherDTO weatherDTO = new WeatherDTO();
        weatherDTO.setCity("City");
        weatherDTO.setTemperature(25.0);
        weatherDTO.setCondition(new ConditionDTO());
        weatherDTO.getCondition().setText("Condition");

        Condition condition = new Condition();
        condition.setText("Condition");

        Weather existingWeather = new Weather();
        existingWeather.setId(1L);
        existingWeather.setCity("City");
        existingWeather.setTemperature(20.0);
        existingWeather.setCondition(condition);

        Weather updatedWeather = new Weather();
        updatedWeather.setId(1L);
        updatedWeather.setCity("City");
        updatedWeather.setTemperature(25.0);
        updatedWeather.setCondition(condition);

        when(weatherRepository.findById(1L)).thenReturn(Optional.of(existingWeather));
        when(conditionService.getConditionByText(weatherDTO.getCondition().getText())).thenReturn(condition);
        when(weatherRepository.save(any(Weather.class))).thenReturn(updatedWeather);

        // Вызов метода
        Weather result = weatherService.updateWeather(1L, weatherDTO);

        // Проверка результата
        assertEquals(updatedWeather, result);
        verify(weatherRepository, times(1)).findById(1L);
        verify(conditionService, times(1)).getConditionByText(weatherDTO.getCondition().getText());
        verify(weatherRepository, times(1)).save(weatherArgumentCaptor.capture());
        Weather capturedWeather = weatherArgumentCaptor.getValue();
        assertEquals("City", capturedWeather.getCity());
        assertEquals(25.0, capturedWeather.getTemperature());
        assertEquals(condition, capturedWeather.getCondition());
    }

    @Test
    void testUpdateWeatherWhenWeatherDoesNotExistShouldThrowBadRequestException() {
        // Подготовка данных
        WeatherDTO weatherDTO = new WeatherDTO();
        weatherDTO.setCity("City");
        weatherDTO.setTemperature(25.0);
        weatherDTO.setCondition(new ConditionDTO());
        weatherDTO.getCondition().setText("Condition");

        when(weatherRepository.findById(1L)).thenReturn(Optional.empty());

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.updateWeather(1L, weatherDTO));
        verify(weatherRepository, times(1)).findById(1L);
        verify(conditionService, never()).getConditionByText(anyString());
        verify(weatherRepository, never()).save(any(Weather.class));
    }

    @Test
    void testDeleteWeatherWhenWeatherExistsShouldDeleteWeather() throws BadRequestException, InternalServerErrorException {
        // Подготовка данных
        Weather existingWeather = new Weather();
        existingWeather.setId(1L);
        existingWeather.setCity("City");

        when(weatherRepository.findById(1L)).thenReturn(Optional.of(existingWeather));

        // Вызов метода
        weatherService.deleteWeather(1L);

        // Проверка результата
        verify(weatherRepository, times(1)).findById(1L);
        verify(weatherRepository, times(1)).delete(any(Weather.class));
    }

    @Test
    void testDeleteWeatherWhenWeatherDoesNotExistShouldThrowBadRequestException() {
        // Подготовка данных
        when(weatherRepository.findById(1L)).thenReturn(Optional.empty());

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.deleteWeather(1L));
        verify(weatherRepository, times(1)).findById(1L);
        verify(weatherRepository, never()).deleteById(anyLong());
    }

    @Test
    void testGetWeatherByIdWhenWeatherExistsShouldReturnWeather() throws BadRequestException, InternalServerErrorException {
        // Подготовка данных
        Weather existingWeather = new Weather();
        existingWeather.setId(1L);
        existingWeather.setCity("City");

        when(weatherRepository.findById(1L)).thenReturn(Optional.of(existingWeather));

        // Вызов метода
        Weather result = weatherService.getWeatherById(1L);

        // Проверка результата
        assertEquals(existingWeather, result);
        verify(weatherRepository, times(1)).findById(1L);
    }

    @Test
    void testGetWeatherByIdWhenWeatherDoesNotExistShouldThrowBadRequestException() {
        // Подготовка данных
        when(weatherRepository.findById(1L)).thenReturn(Optional.empty());

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.getWeatherById(1L));
        verify(weatherRepository, times(1)).findById(1L);
    }

    @Test
    void testGetAllWeathersShouldReturnAllWeathers() throws InternalServerErrorException {
        // Подготовка данных
        List<Weather> weathers = new ArrayList<>();
        weathers.add(new Weather());
        weathers.add(new Weather());

        when(weatherRepository.findAll()).thenReturn(weathers);

        // Вызов метода
        List<Weather> result = weatherService.getAllWeathers();

        // Проверка результата
        assertEquals(2, result.size());
        verify(weatherRepository, times(1)).findAll();
    }

    @Test
    void testGetAllWeathersWhenWeatherRepositoryThrowsExceptionShouldThrowInternalServerErrorException() {
        // Подготовка данных
        when(weatherRepository.findAll()).thenThrow(new RuntimeException());

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.getAllWeathers());
        verify(weatherRepository, times(1)).findAll();
    }

    @Test
    void testFindByTemperatureWhenWeathersExistsShouldReturnWeathers() throws InternalServerErrorException {
        // Подготовка данных
        List<Weather> weathers = new ArrayList<>();
        weathers.add(new Weather());
        weathers.add(new Weather());

        when(weatherRepository.findByTemperature(25.0)).thenReturn(weathers);

        // Вызов метода
        List<WeatherDTO> result = weatherService.findByTemperature(25.0);

        // Проверка результата
        assertEquals(2, result.size());
        verify(weatherRepository, times(1)).findByTemperature(25.0);
    }

    @Test
    void testFindByTemperatureWhenWeatherRepositoryThrowsExceptionShouldThrowInternalServerErrorException() {
        // Подготовка данных
        when(weatherRepository.findByTemperature(25.0)).thenThrow(new RuntimeException());

        // Вызов метода и проверка исключения
        assertThrows(InternalServerErrorException.class, () -> weatherService.findByTemperature(25.0));
        verify(weatherRepository, times(1)).findByTemperature(25.0);
    }
}
