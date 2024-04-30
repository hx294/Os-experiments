/**
 * Time-of-day server listening to port 6013.
 *
 * Figure 3.27
 *
 * @author Silberschatz, Gagne, and Galvin. 
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018.
 */
 
import java.net.*;
import java.io.*;

public class qotdServer
{
	private static final String[] quotes = {
		"我庭の小草萌えいでぬ限りなき天地今やよみがへるらし",
		"春雨や傘高低に渡し舟",
		"雪間より薄紫の芽独活哉",
		"蛸壶やはかなき梦を夏の月"
	};
	public static void main(String[] args)  {
		try {
			ServerSocket sock = new ServerSocket(5575);

			// now listen for connections
			while (true) {
				Socket client = sock.accept();
				// we have a connection
				
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				// write the Date to the socket
				pout.println(quotes[new java.util.Random().nextInt(4)]);

				// close the socket and resume listening for more connections
				client.close();
			}
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
