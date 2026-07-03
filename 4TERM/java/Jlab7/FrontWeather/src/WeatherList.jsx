import { useState, useEffect } from 'react';
import axios from 'axios';

const WeatherList = () => {
    const [weathers, setWeathers] = useState([]);
    const [newWeather, setNewWeather] = useState({
        city: '',
        temperature: '',
        condition: {
            text: ''
        }
    });
    const [notification, setNotification] = useState(null);
    const [currentPage, setCurrentPage] = useState(1);
    const [searchQuery, setSearchQuery] = useState('');
    const itemsPerPage = 7;

    useEffect(() => {
        fetchWeathers();
    }, []);

    const fetchWeathers = () => {
        axios.get('https://weatherservice-upd.onrender.com/weather')
            .then(response => {
                setWeathers(response.data);
            })
            .catch(() => {
                showNotification('Error fetching weather data', 'error');
            });
    };

    const searchWeathers = () => {
        axios.get(`https://weatherservice-upd.onrender.com/weather/city/${searchQuery}`)
            .then(response => {
                if (response.status === 200) {
                    setWeathers([response.data]);
                    setCurrentPage(1);
                } else {
                    showNotification('City not found', 'error');
                }
            })
            .catch(() => {
                showNotification('Error searching weather data', 'error');
            });
    };

    const updateWeather = (weather) => {
        axios.put(`https://weatherservice-upd.onrender.com/weather/${weather.id}`, weather)
            .then(response => {
                setWeathers(weathers.map(w => (w.id === weather.id ? response.data : w)));
                showNotification('Weather updated successfully', 'success');
            })
            .catch(() => {
                showNotification('Error updating weather', 'error');
            });
    };

    const deleteWeather = (id) => {
        axios.delete(`https://weatherservice-upd.onrender.com/weather/${id}`)
            .then(() => {
                setWeathers(weathers.filter(weather => weather.id !== id));
                showNotification('Weather deleted successfully', 'success');
            })
            .catch(() => {
                showNotification('Error deleting weather', 'error');
            });
    };

    const addWeather = () => {
        axios.post('https://weatherservice-upd.onrender.com/weather', newWeather)
            .then(response => {
                setWeathers([...weathers, response.data]);
                setNewWeather({
                    city: '',
                    temperature: '',
                    condition: {
                        text: ''
                    }
                });
                showNotification('Weather added successfully', 'success');
            })
            .catch(() => {
                showNotification('Error adding weather', 'error');
            });
    };

    const showAllWeathers = () => {
        fetchWeathers();
        setSearchQuery('');
    };

    const showNotification = (message, type) => {
        setNotification({ message, type });
        setTimeout(() => {
            setNotification(null);
        }, 3000);
    };

    const handlePageChange = (page) => {
        setCurrentPage(page);
    };

    const renderPagination = () => {
        const totalPages = Math.ceil(weathers.length / itemsPerPage);
        let startPage, endPage;
        if (totalPages <= 5) {
            startPage = 1;
            endPage = totalPages;
        } else {
            if (currentPage <= 3) {
                startPage = 1;
                endPage = 5;
            } else if (currentPage + 2 >= totalPages) {
                startPage = totalPages - 4;
                endPage = totalPages;
            } else {
                startPage = currentPage - 2;
                endPage = currentPage + 2;
            }
        }

        const pages = Array.from({ length: (endPage + 1) - startPage }, (_, i) => startPage + i);

        return (
            <div className="pagination">
                <button onClick={() => handlePageChange(currentPage - 1)} disabled={currentPage === 1}>Назад</button>
                {pages.map(page =>
                    <button
                        key={page}
                        onClick={() => handlePageChange(page)}
                        className={currentPage === page ? 'active-page' : ''}
                    >
                        {page}
                    </button>
                )}
                <button onClick={() => handlePageChange(currentPage + 1)} disabled={currentPage === totalPages}>Вперед</button>
            </div>
        );
    };

    const currentWeathers = weathers.slice((currentPage - 1) * itemsPerPage, currentPage * itemsPerPage);

    return (
        <div className="wrapper">
            {notification && (
                <div className={`notification ${notification.type}`}>
                    {notification.message}
                </div>
            )}
            <div className="weather-container">
                <div className="search-container">
                    <button onClick={showAllWeathers}>ALL</button>
                    <input
                        type="text"
                        value={searchQuery}
                        placeholder="SEARCH"
                        onChange={(e) => setSearchQuery(e.target.value)}
                    />
                    <button onClick={searchWeathers}>SEARCH</button>
                </div>
                <div className="weather-list">
                    {currentWeathers.map(weather => (
                        <div key={weather.id} className="weather-item">
                            <input
                                type="text"
                                value={weather.city}
                                onChange={(e) => setWeathers(weathers.map(w => (w.id === weather.id ? { ...w, city: e.target.value } : w)))}
                            />
                            <input
                                type="text"
                                value={weather.temperature}
                                onChange={(e) => setWeathers(weathers.map(w => (w.id === weather.id ? { ...w, temperature: e.target.value } : w)))}
                            />
                            <input
                                type="text"
                                value={weather.condition.text}
                                onChange={(e) => setWeathers(weathers.map(w => (w.id === weather.id ? { ...w, condition: { text: e.target.value } } : w)))}
                            />
                            <button onClick={() => updateWeather(weather)}>CHANGE</button>
                            <button onClick={() => deleteWeather(weather.id)}>DELETE</button>
                        </div>
                    ))}
                </div>
                {renderPagination()}
                <div className="new-weather">
                    <input
                        type="text"
                        value={newWeather.city}
                        placeholder="City"
                        onChange={(e) => setNewWeather({ ...newWeather, city: e.target.value })}
                    />
                    <input
                        type="text"
                        value={newWeather.temperature}
                        placeholder="Temperature"
                        onChange={(e) => setNewWeather({ ...newWeather, temperature: e.target.value })}
                    />
                    <input
                        type="text"
                        value={newWeather.condition.text}
                        placeholder="Condition"
                        onChange={(e) => setNewWeather({ ...newWeather, condition: { text: e.target.value } })}
                    />
                    <button onClick={addWeather}>ADD</button>
                </div>
            </div>
        </div>
    );
};

export default WeatherList;