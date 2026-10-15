public class ArrayOfArrayOfInt {
	public static void main(String[] args) {
		int[][] arrayOfArrayOfInt = new int[10][];
		for (int i=0; i<10; i++) { 
			arrayOfArrayOfInt[i] = new int[5];
			for (int j=0; j<5; j++)
				arrayOfArrayOfInt[i][j] = i * 10 + j;
		}
		int[][] triangleOfArrayOfInt = new int[10][];
		for (int i=0; i<10; i++) {
			triangleOfArrayOfInt[i] = new int[i];
			for (int j=0; j<i; j++) 
				triangleOfArrayOfInt[i][j] = i * 10 + j;
		}
		for (int i=0; i<arrayOfArrayOfInt.length; i++) {
			System.out.println("");
			for (int j=0; j<arrayOfArrayOfInt[i].length; j++)
				System.out.print(arrayOfArrayOfInt[i][j] + " ");
		}
		for (int i=0; i<triangleOfArrayOfInt.length; i++) {
			System.out.println("");
			for (int j=0; j<triangleOfArrayOfInt[i].length; j++) 
				System.out.print(triangleOfArrayOfInt[i][j] + " ");
		}
		System.out.println("");
	}
}