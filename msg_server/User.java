import java.util.List;
import java.util.ArrayList;
import java.io.Serializable;

public class User implements Serializable {

	private String username;
	private String password;
	private List<Message> messages;

	User(String username, String password) {
		this.username = username;
		this.password = password;
		this.messages = new ArrayList<Message>();
	}

	public String get_username() {
		return this.username;
	}

	public List<Message> get_message_list() {
		return this.messages;
	}

	public Message get_message(int pos) {
		return this.messages.get(pos);
	}

	public Message add_message(Message msg) {
		this.messages.add(msg);

		return msg;
	}
}
