import java.util.ArrayList;
import java.util.List;

public class hexHelper {

    public hexHelper() {
    }

    public static List<int[]> hexToBinary(String hexInput) {

        java.util.Map<Character, int[]> hexDcn = new java.util.HashMap<>();
        hexDcn.put('0', new int[] { 0, 0, 0, 0 });
        hexDcn.put('1', new int[] { 0, 0, 0, 1 });
        hexDcn.put('2', new int[] { 0, 0, 1, 0 });
        hexDcn.put('3', new int[] { 0, 0, 1, 1 });
        hexDcn.put('4', new int[] { 0, 1, 0, 0 });
        hexDcn.put('5', new int[] { 0, 1, 0, 1 });
        hexDcn.put('6', new int[] { 0, 1, 1, 0 });
        hexDcn.put('7', new int[] { 0, 1, 1, 1 });
        hexDcn.put('8', new int[] { 1, 0, 0, 0 });
        hexDcn.put('9', new int[] { 1, 0, 0, 1 });
        hexDcn.put('A', new int[] { 1, 0, 1, 0 });
        hexDcn.put('B', new int[] { 1, 0, 1, 1 });
        hexDcn.put('C', new int[] { 1, 1, 0, 0 });
        hexDcn.put('D', new int[] { 1, 1, 0, 1 });
        hexDcn.put('E', new int[] { 1, 1, 1, 0 });
        hexDcn.put('F', new int[] { 1, 1, 1, 1 });

        List<int[]> binaryOutput = new ArrayList<>();
        for (int i = 0; i < hexInput.length(); i++) {
            char c = hexInput.charAt(i);
            if (hexDcn.containsKey(c)) {
                binaryOutput.add(hexDcn.get(c));
            }
        }
        return binaryOutput;
    }
}