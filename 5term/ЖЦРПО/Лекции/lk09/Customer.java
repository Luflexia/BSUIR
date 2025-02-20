import java.util.Enumeration;
import java.util.Vector;

/**
 * Created by Natalia Iskra on 10/25/15.
 */
public class Customer {
    private String _name;
    private Vector _rentals = new Vector();

    public Customer(String name) {
        _name = name;
    }

    public void addRental(Rental arg) {
        _rentals.addElement(arg);
    }

    public String getName() {
        return _name;
    }

    //метод для рефакторинга
    public String statement() {
        double totalAmount = 0;
        int frequentRenterPoints = 0;
        Enumeration rentals = _rentals.elements();
        String result = "Учет аренды для: " + getName() + "\n";
        while (rentals.hasMoreElements()) {
            double thisAmount = 0;
            Rental each = (Rental) rentals.nextElement();

            //определить сумму для каждой строки
            switch (each.getMovie().getPriceCode()) {
                case Movie.REGULAR:
                    thisAmount += 2;
                    if (each.getDaysRented() > 2)
                        thisAmount += (each.getDaysRented() - 2) * 1.5;
                    break;
                case Movie.NEW_RELEASE:
                    thisAmount += each.getDaysRented() * 3;
                    break;
                case Movie.CHILDRENS:
                    thisAmount += 1.5;
                    if (each.getDaysRented() > 3)
                        thisAmount += (each.getDaysRented() - 3) * 1.5;
                    break;
            }

            // добавить очки для активного арендатора
            frequentRenterPoints++;
            // бонус за аренду новинки на два дня
            if ((each.getMovie().getPriceCode() == Movie.NEW_RELEASE) &&
                    each.getDaysRented() > 1) {
                frequentRenterPoints++;
            }
            //показать результаты для этой аренды
            result += "\t" + each.getMovie().getTitle() + "\t" +
                    String.valueOf(thisAmount) + "\n";
            totalAmount += thisAmount;
        }
        //добавить нижний колонтитул
        result += "Сумма задолженности составляет: " +
                String.valueOf(totalAmount) + "\n";
        result += "Вы заработали " + String.valueOf(frequentRenterPoints) +
                " очков за активность";
        return result;
    }
}
