package com.app.weather.component;

import org.springframework.stereotype.Component;
import java.util.HashMap;
import java.util.Map;

@Component
public class CacheComponent {

    private final Map<String, Object> hashMap = new HashMap<>();
    private final CustomLogger customLogger;

    public CacheComponent(CustomLogger customLogger) {
        this.customLogger = customLogger;
    }

    public void put(String key, Object value) {
    hashMap.put(key, value);
    int maxSize = 100;
    if (hashMap.size() > maxSize) {
            String oldestKey = hashMap.keySet().iterator().next();
            hashMap.remove(oldestKey);
        }
        customLogger.cachePut();
    }

    public Object get(String key) {
        return hashMap.get(key);
    }

    public void remove(String key) {
        hashMap.remove(key);
        customLogger.cacheRemove();
    }
}
