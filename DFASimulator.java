import java.util.*;
import java.io.*;

public class DFASimulator {
    // list of integer final states
    private static ArrayList<Integer> FinalStates = new ArrayList<Integer>();
    // list of transitions: maps [prev_state, input] ==> final_state
    private static HashMap<String, Integer> Transitions = new HashMap<String, Integer>();
    // name/directory of file to read from.
    private String filename;

    DFASimulator(String filename) {
        this.filename = filename;
    }

    private static void getFinalStates(String firstLine) {
        // split by whitespace, convert to integer and populate the final states
        for (String s : firstLine.split("\\s+")) {
            FinalStates.add(Integer.parseInt(s));
        }
    }

    private static void getTransitions(String fileContent) {
        // ignore empty lines
        if(fileContent.trim().length() == 0) return;

        int index = 0;
        // key = [current_state, input]
        String[] key = new String[2];
        for (String s : fileContent.split("")) {
            switch (index) {
                case 0:
                    key[0] = s;
                    index++;
                    break;
                case 1:
                    key[1] = Character.toString(s.charAt(0));
                    index++;
                    break;
                case 2:
                    Transitions.put(String.join("", key), Integer.parseInt(s));
                    break;
                default:
                    System.out.println("There is an error in the DFA file.");
            }
        }
    }

    private void readFile() {
        int lineNumber = 1;
        try {
            FileInputStream inputStream = new FileInputStream(filename);
            Scanner scanner = new Scanner(inputStream); 
            
            // read file and call getFinalStates if lineNumber is 1 or getTransitions otherwise.
            while (scanner.hasNextLine()) {
                String text = scanner.nextLine();

                if (lineNumber == 1) {
                    getFinalStates(text);
                } else {
                    getTransitions(text);
                }

                lineNumber++;
            }

            scanner.close();
        } catch (IOException exception) {
            exception.printStackTrace();
        }
    }

    public void checkDFAInput(String str) {
        this.readFile();
        
        int currentState = 0;

        for(int i = 0; i < str.length(); i++) {
            char alphabet = str.charAt(i);
            String key = String.valueOf(currentState) + Character.toString(alphabet);
            try {
              currentState = Transitions.get(key);
            } catch(Exception exception) {
              System.out.printf("Error: \"%s\" is not in FSM alphabet. \n", Character.toString(alphabet));
              return;
            }
        }

        if (FinalStates.contains(currentState)) {
            System.out.println("Accepted");
        } else {
            System.out.println("Rejected");
        }
    }
}
