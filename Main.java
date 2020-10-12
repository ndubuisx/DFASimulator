public class Main {
  public static void main(String[] args) {
      Scanner scanner = new Scanner(System.in);
      System.out.print("Enter filename: ");
      String filename = scanner.nextLine();

      DFASimulator dfaSimulator = new DFASimulator(filename);

      String str = "";

      while(!str.equals("quit")) {
          System.out.print("Enter string to test against DFA: ");
          str = scanner.nextLine();
          if (!str.equals("quit")) {
            dfaSimulator.checkDFAInput(str);
          }
      }

      scanner.close();
  }
}
