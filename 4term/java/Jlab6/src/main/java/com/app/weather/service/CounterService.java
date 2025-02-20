package com.app.weather.service;
import org.springframework.stereotype.Service;
import java.util.concurrent.atomic.AtomicInteger;

@Service
public class CounterService {
    private final AtomicInteger count;

    public CounterService() {
        count = new AtomicInteger(0);
    }

    public int incrementAndGet() {
        return count.incrementAndGet();
    }

    public int get() {
        return count.get();
    }
}
