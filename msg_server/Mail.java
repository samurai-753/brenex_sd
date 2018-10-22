import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Mail extends Remote {

	public int add_user(User u) throws RemoteException;

	public String list_users() throws RemoteException;

}
