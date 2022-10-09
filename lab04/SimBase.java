import java.util.Scanner;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;


public class SimBase {

    // memory and registers
    public byte[] M;
    public byte[] R;

    // control registers; do not modify these directly
    private byte _pc; // program counter (current memory)
    private byte _ir; // current instruction
    private byte _rsp; // stack pointer

    /**
     * Constructor, initializing memory from a file
     */
    public SimBase(Path filepath) throws java.io.IOException {
        M = new byte[256];
        R = new byte[4];
        _rsp = (byte) 0x00;
        Scanner s = new Scanner(Files.newInputStream(filepath));
        int i = 0;
        while (s.hasNext()) {
            M[i] = (byte) s.nextShort(16); // cast because Java has signed bytes
            i += 1;
        }
    }

    /**
     * Constructor, initializing memory from bytes on the command line
     */
    public SimBase(String[] bytes) {
        M = new byte[256];
        R = new byte[4];
        _rsp = (byte) 0x00;
        for (int i = 0; i < 256 && i < bytes.length; i += 1) {
            M[i] = (byte) Short.parseShort(bytes[i], 16); // cast because Java has signed bytes
        }
    }

    // Simulator Functions

    /**
     * Implements one clock cycle
     */
    public void cycle() {
        _ir = M[_pc];
        _pc = (byte) execute(_ir, _pc);
    }

    /**
     * Handles a single instruction, returning the new PC.
     */
    public int execute(byte instruction, byte oldPC) {
        // to do: add instructions here
        int icode = getBits(instruction, 4, 8); // 1 reserved + 3 icode
        int a = getBits(instruction, 2, 4);
        int b = getBits(instruction, 0, 2);

        switch (icode) {
            case 0:                     // Set
                R[a] = R[b];
                break;
            case 1:                     // Add
                R[a] += R[b];
                break;
            case 2:                     // And
                R[a] &= R[b];
                // make unsigned?
                break;
            case 3:                     // Read from address
                R[a] = M[unsigned(R[b])];
                break;
            case 4:                     // Write to address
                M[unsigned(R[b])] = R[a];
                break;
            case 5:
                switch (b) {
                    case 0:             // Flip bits
                        R[a] = (byte) ~R[a];
                        break;
                    case 1:             // Negate sign
                        R[a] = (byte) -R[a];
                        break;
                    case 2:             // Logical not
                        R[a] = (byte) ((R[a] == 0) ? 1 : 0); // rA = ~rA
                        break;
                    case 3:             // Save current address
                        R[a] = oldPC;
                        break;
                }
                break;
            case 6:
                int i = unsigned(oldPC + 1);
                switch (b) {
                    case 0:             // Read next literal
                        R[a] = M[i];
                        break;
                    case 1:             // Add next literal
                        R[a] += M[i];
                        break;
                    case 2:             // And next literal
                        R[a] &= M[i];
                        break;
                    case 3:             // Read next address
                        R[a] = M[unsigned(M[i])];
                        break;
                }
                return oldPC + 2;
            case 7:                     // Conditional jump
                if (R[a] <= 0) return R[b];
                break;
            case 8:
                switch (b) {
                    case 0:             // Push to stack
                        push(R[a]);
                        break;
                    case 1:             // Pop from stack
                        R[a] = pop();
                        break;
                    case 2:             // Function call
                        push(M[unsigned(oldPC + 2)]);
                        return M[unsigned(oldPC + 1)];
                    case 3:             // Unconditional jump
                        return pop();
                }
                break;
            default:
                return oldPC; // halt
        }

        return oldPC + 1;
    }
    

    // Helper Functions

    private void push(byte b) {
        M[unsigned(--_rsp)] = b;
    }

    private byte pop() {
        return M[unsigned(_rsp++)];
    }

    /**
     * Converts a decimal integer to a binary string with the given width.
     */
    public static String toBin(int n, int width) {
        String ans = "";
        for (int i = 0; i < width; i += 1) {
            ans = (char) ('0' + (n & 1)) + ans;
            n >>= 1;
        }
        return ans;
    }

    /**
     * Returns the bits of number between idx1 (inclusive) and idx2 (exclusive) as an integer.
     */
    public static int getBits(int number, int idx1, int idx2) {
        int low = idx1 < idx2 ? idx1 : idx2;
        int num = idx1 < idx2 ? idx2 - idx1 : idx1 - idx2;
        return (number >> low) & ((1 << num) - 1);
    }

    /**
     * Converts a signed integer into an unsigned integer.
     */
    public static int unsigned(int b) {
        return b & 0xFF;
    }

    // Main/Display Method

    /**
     * Displays all processor variables to command line.
     */
    public void showState() {
        System.out.println("----------------------------------------");
        System.out.printf("Last Instruction = 0b%s (0x%02x)\n", toBin(_ir, 8), _ir);
        for (int i = 0; i < 4; i += 1) {
            System.out.printf("Register %s = 0b%s (0x%02x)\n", toBin(i, 2), toBin(R[i], 8), R[i]);
        }
        System.out.printf("Next PC = 0b%s (0x%02x)\n", toBin(_pc, 8), _pc);
        System.out.printf("Stack Pointer = 0b%s (0x%02x)\n", toBin(_rsp, 8), _rsp);
        System.out.println("//////////////////////// Memory \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
        for (int i = 0; i < 256; i += 16) {
            System.out.printf("0x%02x-%02x: ", i, i + 15);
            for (int j = 0; j < 16; j += 1) {
                System.out.printf("%02x ", M[i + j]);
            }
            System.out.println();
            boolean all0 = true;
            for (int j = i + 16; j < M.length; j += 1) all0 &= (M[j] == 0);
            if (all0) break;
        }
        System.out.println("----------------------------------------");
    }

    public static void main(String[] args) {
        if (args.length <= 0) {
            System.err.println("USAGE: java SimBase memory.txt\n    where memory.txt is a set of bytes in hex");
            System.err.println("USAGE: java SimBase byte [byte, byte, ...]\n    where the bytes are in hex and will be loaded into memory before running");
            System.exit(1);
        }
        SimBase simulator;
        try { // load program from file (ex: program/halt)
            Path p = Paths.get(args[0]).toRealPath();
            simulator = new SimBase(p);
        } catch (java.io.IOException ex) { // run byte code (ex: 00 00 00 80)
            simulator = new SimBase(args);
        }
        simulator.showState();
        Scanner keyboard = new Scanner(System.in);
        while (true) {
            System.out.print("Take how many steps (0 to exit, default 1)? ");
            String n = keyboard.nextLine(); // count the number of bytes (ex: 18)
            int num = 1;
            try {
                num = Integer.parseInt(n);
            } catch (NumberFormatException ex) {
            }
            if (num <= 0) break;
            for (int i = 0; i < num; i += 1) {
                simulator.cycle();
                simulator.showState();
            }
        }
    }
}
