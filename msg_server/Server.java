import java.util.List;
import java.util.ArrayList;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server implements Mail {

	private List<User> users;

	Server() {
		this.users = new ArrayList<>();
	}

	public int add_user(User u) throws RemoteException {
		this.users.add(u);
		System.out.println("add_user");

		return 0;
	}

	public String list_users() throws RemoteException {
		String s = "";

		for(User u : this.users) {
			s += u.get_username();
			s += " ";
		}
		
		return s;
	}

	public static void main(String args[]) {
		try {
			Server server = new Server();

			Mail stub = (Mail) UnicastRemoteObject.exportObject(server, 0);
			Registry registry = LocateRegistry.getRegistry();
			registry.rebind("Mail", stub);

			System.out.println("Server Ready");
		} catch (Exception e) {
			System.err.println("Server exception: " + e.toString());
		}
	}

}
