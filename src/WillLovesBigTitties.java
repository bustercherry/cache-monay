import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.util.StringTokenizer;

public class WillLovesBigTitties
{

	public static void main(String[] args) throws NumberFormatException, IOException
	{
		
		BufferedReader inputStream = null;
		String l;
		char refType = ' ';
		long address = 0;
		int numBytes = 0;

		try
		{
			inputStream = new BufferedReader(new InputStreamReader(System.in));

			while ((l = inputStream.readLine()) != null)
			{
				StringTokenizer st = new StringTokenizer(l);
				if (st.hasMoreTokens()) refType = st.nextToken().charAt(0);
				if (st.hasMoreTokens()) address = new BigInteger(st.nextToken(), 16).longValue();
				if (st.hasMoreTokens()) numBytes = Integer.parseInt(st.nextToken());
			
				
				System.out.println("Reference: " + refType + " Address: " + address + " Bytes: " + numBytes);
			}

		}
		finally
		{
			if (inputStream != null)
			{
				inputStream.close();
			}
		}

	}

}
