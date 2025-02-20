# Используем Maven для сборки проекта
FROM maven AS build 

# Устанавливаем рабочую директорию
WORKDIR /app 

# Копируем файл pom.xml
COPY ./pom.xml ./ 

# Загружаем зависимости
RUN mvn dependency:go-offline -DexcludeArtifactIds=maven-checkstyle-plugin 

# Копируем исходный код
COPY ./src ./src 

# Собираем проект, пропуская тесты и используя профиль docker
RUN mvn clean install -DskipTests -Pdocker 

# Используем OpenJDK для запуска приложения
FROM openjdk:21-jdk-slim AS run 

# Устанавливаем переменную окружения для URL базы данных
ENV DATABASE_URL=db-container:5432 
# Устанавливаем рабочую директорию
WORKDIR /app 

# Копируем скомпилированный JAR файл из предыдущего контейнера
COPY --from=build /app/target/*.jar /app/web.jar 

# Открываем порт 8080
EXPOSE 8080 

# Запускаем приложение
CMD ["java", "-jar", "web.jar"] 
