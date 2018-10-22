import java.util.Scanner;
import java.rmi.RemoteException;
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;

public class Client {

	private static void show_menu() {
		System.out.println("-----------");
		System.out.println("1 - Add user");
		System.out.println("2 - List users");
		System.out.println("0 - Quit");
		System.out.println("-----------");
	}

	public static void list_users(Mail mail) throws RemoteException {
		System.out.println(mail.list_users());
	}

	public static void add_user(Mail mail) throws RemoteException {
		Scanner scan = new Scanner(System.in);

		System.out.print("Username: ");
		String name = scan.nextLine();

		System.out.print("Pass: ");
		String pass = scan.nextLine();

		mail.add_user(new User(name, pass));

		scan.close();
	}

	public static void main(String args[]) {
		try {
			Registry registry = LocateRegistry.getRegistry();
			Mail mail = (Mail) registry.lookup("Mail");

			list_users(mail);
			add_user(mail);
			list_users(mail);


		} catch (Exception e) {
			System.err.println("Cliente exception: " + e.toString());
			e.printStackTrace();
		}
	}

}
