import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client {

    public static void main(String[] args) {
        try (Socket clientSocket = new Socket("localhost", 4004);
             BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            // читать соообщения с сервера
             BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            // писать туда же
             BufferedWriter out = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()))
            ) {
                System.out.println("Вы что-то хотели сказать? Введите это здесь:");
                // если соединение произошло и потоки успешно созданы - мы можем
                //  работать дальше и предложить клиенту что то ввести
                // если нет - вылетит исключение
                String word = reader.readLine(); // ждём пока клиент что-нибудь
                // не напишет в консоль
                out.write(word + "\n"); // отправляем сообщение на сервер
                out.flush();
                String serverWord = in.readLine(); // ждём, что скажет сервер
                System.out.println(serverWord); // получив - выводим на экран
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}