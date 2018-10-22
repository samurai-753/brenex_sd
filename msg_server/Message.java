import java.util.Date;
import java.io.Serializable;


public class Message implements Serializable {

	private String sender;
	private Date date;
	private String title;
	private String text;

	Message(String sender, Date date, String title, String text) {
		this.sender = sender;
		this.date = date;
		this.title = title;
		this.text = text;
	}

	public String get_sender() {
		return this.sender;
	}

	private Date get_date() {
		return this.date;
	}

	private String get_title() {
		return this.title;
	}

	private String get_text() {
		return this.text;
	}

}
