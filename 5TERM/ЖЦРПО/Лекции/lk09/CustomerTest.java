import static org.junit.Assert.*;
import org.junit.Test;

/**
 * Created by Natalia Iskra on 10/25/15.
 */
public class CustomerTest {

    final static String NO_RENTALS_STATEMENT = "Учет аренды для: Natalia Iskra\n" +
            "Сумма задолженности составляет: 0.0\n" +
            "Вы заработали 0 очков за активность";



    @Test
    public void testNothing() {
    }

    @Test
    public void testNoRentals() {
        Customer customer = new Customer("Natalia Iskra");
        assertEquals("Неправильный вывод для пустого заказа", NO_RENTALS_STATEMENT, customer.statement());
    }

    @Test
    public void testRegularRental() {
        String expected = "Учет аренды для: Natalia Iskra\n" +
                "\tStar Wars 4\t2.0\n" +
                "Сумма задолженности составляет: 2.0\n" +
                "Вы заработали 1 очков за активность";

        Customer customer = new Customer("Natalia Iskra");
        Movie movie = new Movie("Star Wars 4", Movie.REGULAR);
        Rental rental = new Rental(movie, 2);
        customer.addRental(rental);

        assertEquals("Неправильный вывод для обычного заказа на 2 дня", expected, customer.statement());
    }

    @Test
    public void testNewRental() {
        String expected = "Учет аренды для: Natalia Iskra\n" +
                "\tStar Wars 7\t6.0\n" +
                "Сумма задолженности составляет: 6.0\n" +
                "Вы заработали 2 очков за активность";

        Customer customer = new Customer("Natalia Iskra");
        Movie movie = new Movie("Star Wars 7", Movie.NEW_RELEASE);
        Rental rental = new Rental(movie, 2);
        customer.addRental(rental);

        assertEquals("Неправильный вывод для новинки на 2 дня", expected, customer.statement());
    }

    @Test
    public void testChildrenRental() {
        String expected = "Учет аренды для: Natalia Iskra\n" +
                "\tStar Wars 1\t1.5\n" +
                "Сумма задолженности составляет: 1.5\n" +
                "Вы заработали 1 очков за активность";

        Customer customer = new Customer("Natalia Iskra");
        Movie movie = new Movie("Star Wars 1", Movie.CHILDRENS);
        Rental rental = new Rental(movie, 2);
        customer.addRental(rental);

        assertEquals("Неправильный вывод для детского фильма на 2 дня", expected, customer.statement());
    }

    @Test
    public void testRegularRentalExpired() {
        String expected = "Учет аренды для: Natalia Iskra\n" +
                "\tStar Wars 4\t6.5\n" +
                "Сумма задолженности составляет: 6.5\n" +
                "Вы заработали 1 очков за активность";

        Customer customer = new Customer("Natalia Iskra");
        Movie movie = new Movie("Star Wars 4", Movie.REGULAR);
        Rental rental = new Rental(movie, 5);
        customer.addRental(rental);

        assertEquals("Неправильный вывод для обычного заказа на 5 дней", expected, customer.statement());
    }



}
