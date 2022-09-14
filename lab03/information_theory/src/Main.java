import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Random;
import java.util.Scanner;

/**
 * Eric Weng
 * Lab 03
 * Partner: Wilson Li
 */
public class Main {

    static final Scanner sc = new Scanner(System.in);

    public static void main(String[] args) {
        // Read file
        String filename = getUserInput("Choose a file to read");
//        String filename = "src/diff_match_patch.txt";
        String text = readFile(filename);
        System.out.printf("The file is %d characters long\n.", text.length());

        // Make guesses
        int correct = 0;
//        int rounds = 5;
        int rounds = 20;
        for (int i = 0; i < rounds; i++) {
            if (guessChar(text)) {
                System.out.println("Correct");
                correct++;
            } else {
                System.out.println("Incorrect");
            }
        }
        System.out.printf("You correctly guessed %d out of %d times.\n", correct, rounds);

        // Calculate entropy
        double invRatio = (double) rounds / correct;
        double entropy = Math.log(invRatio) / Math.log(2.0);
        System.out.printf("The estimated bits of entropy is %.2f.\n", entropy);

        sc.close();
    }

    static String getUserInput(String prompt) {
        System.out.print(prompt + ": ");
        return sc.nextLine();
    }

    static String readFile(String filename) {
        System.out.println("Reading file " + filename);
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            StringBuilder str = new StringBuilder();
            String line = reader.readLine();
            while (line != null) {
                str.append(line);
                line = reader.readLine();
            }
            return str.toString();
        } catch (Exception e) {
            System.out.println("Error reading file " + filename);
            System.exit(1);
            return null;
        }
    }

    static boolean guessChar(String text) {
        // Pick an index
        int index = randomNumber(50, text.length() - 50);
        char target = text.charAt(index);
        String sstr = text.substring(index - 50, index);
        System.out.printf("[%s]\n", sstr);

        // Make user guess
        String guess = "";
        while (guess.length() == 0) guess = getUserInput("Guess the next character");
        char nextChar = guess.charAt(0);
        return nextChar == target;
    }

    // Random integer in [min, max)
    static int randomNumber(int min, int max) {
        Random rand = new Random();
        return rand.nextInt(max - min) + min;
    }

}
