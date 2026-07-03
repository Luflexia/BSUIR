package com.app.weather.component;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

@Component
public class CustomLogger {

    private static final Logger LOGGER = LoggerFactory.getLogger(CustomLogger.class);

    public void error(String text) {
        LOGGER.error(text);
    }

    public void info(String text) {
        LOGGER.info(text);
    }

    public void cachePut() {
        LOGGER.info("Кэш обновлен (put)");
    }

    public void cacheRemove() {
        LOGGER.info("Кэш обновлен (remove)");
    }
}
