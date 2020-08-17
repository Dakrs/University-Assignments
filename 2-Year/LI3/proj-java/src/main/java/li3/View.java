package li3;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.List;

import li3.Controller.*;

/**
 * Classe que gere a representação gráfica do programa
 */
public class View extends JFrame{

    private JTextField pathBox = null;
    private JComboBox<String> dumpList = null;
    private List<JButton> buttons = null;
    private JPanel load = null;
    private JPanel queries = null;
    private JPanel input = null;
    private JPanel resposta = null;

    /**
     * Construtor sem parâmetros da classe View
     */
    public View(){
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        pathBox("/home/random/Desktop/dump/");






    }

    /**
     * Método que insere na janela do programa um campo onde especificar a localização da dump
     * @param str String com o texto que aparece inicialmente no campo para indicar a dump
     */
    public void pathBox(String str){
        setTitle("Filepath of the dump directory");
        setLayout(new FlowLayout(FlowLayout.LEFT,10,10));
        setMinimumSize(new Dimension(600,80));
        setResizable(false);

        load = new JPanel();
        load.setLayout(new GridLayout(0,2,20,5));
        load.setPreferredSize(new Dimension(580,30));
        add(load);

        pathBox = new JTextField(str);
        load.add(pathBox);


        dumpList = new JComboBox<String>();
        load.add(dumpList);
    }

    /**
     * Método que devolve o texto presente no campo para especificar a localização da dump
     */
    public String getPath(){
        return pathBox.getText();
    }

    /**
     * Método que insere na drop list as comunidades à escolha na pasta do dump, das quais se pode fazer load
     * @param dumps Comunidades à escolha do utilizador
     */
    public void addDumps(String[] dumps){
        setTitle("Choose dump");
        dumpList.removeAllItems();
        for(String s : dumps)
            dumpList.addItem(s);



    }

    /**
     * Método que devolve a comunidade selecionada para fazer load
     */
    public String getSelectedDump(){
        try {
            return dumpList.getSelectedItem().toString();
        }
        catch(NullPointerException e) {
            return "";
        }
    }

    /**
     * Método que associa ao campo para especificar a localização da dump um método do controlador, para este poder ser chamado
     * @param pathListener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addPathListener(PathListener pathListener){
        this.pathBox.addActionListener(pathListener);
    }

    /**
     * Método que associa à drop list para escolher a comunidade a dar load um método do controlador, para este poder ser chamado
     * @param dumpListener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addDumpListener(DumpListener dumpListener){
        this.dumpList.addActionListener(dumpListener);
    }

    /**
     * Método que associa ao botão da query 1 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery1Listener(Query1Listener listener){
        this.buttons.get(0).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 2 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery2Listener(Query2Listener listener){
        this.buttons.get(1).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 3 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery3Listener(Query3Listener listener){
        this.buttons.get(2).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 4 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery4Listener(Query4Listener listener){
        this.buttons.get(3).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 5 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery5Listener(Query5Listener listener){
        this.buttons.get(4).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 6 um método do controlador, para este poder ser chamado
     */
    public void addQuery6Listener(Query6Listener listener){
        this.buttons.get(5).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 7 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery7Listener(Query7Listener listener){
        this.buttons.get(6).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 8 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery8Listener(Query8Listener listener){
        this.buttons.get(7).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 9 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery9Listener(Query9Listener listener){
        this.buttons.get(8).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 10 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery10Listener(Query10Listener listener){
        this.buttons.get(9).addActionListener(listener);
    }

    /**
     * Método que associa ao botão da query 11 um método do controlador, para este poder ser chamado
     * @param listener Instância da classe que define o comportamento a tomar quando o elemento for ativado
     */
    public void addQuery11Listener(Query11Listener listener){
        this.buttons.get(10).addActionListener(listener);
    }

    /**
     * Método que gere a representação de um painel para apresentar a resposta a uma query
     * @param s resposta recebida do Controller
     */
    public void answer(String s){
        setSize(600,800);
        if (resposta != null) remove(resposta);
        resposta = new JPanel(new BorderLayout());
        resposta.setPreferredSize(new Dimension(580, 250));
        resposta.setMaximumSize(new Dimension(580,250));
        setLocationRelativeTo(null);

        JTextPane texto = new JTextPane();
        resposta.add(texto);
        texto.setEditable(false);
        texto.setText(s);

        JScrollPane sp = new JScrollPane(texto);
        resposta.add(sp);

        add(resposta);
        this.validate();

    }

    /**
     * Método que coloca a ganela no modo "loading"
     */
    public void isLoading(){
        this.remove(dumpList);
        this.setTitle("Loading...");

    }

    /**
     * Método que gere a apresentação dos botões com a finalidade de escolher uma query a chamar
     */
    public  void mainMenu(){
        if (queries != null) return;
        setTitle(getSelectedDump());
        setMinimumSize(new Dimension(600, 250));
        setLocationRelativeTo(null);

        buttons = new ArrayList<JButton>();
        queries = new JPanel();
        queries.setLayout(new GridLayout(0,4,20,20));
        queries.setPreferredSize(new Dimension(580, 160));
        add(queries);

        JButton button;

        for(int i = 0; i < 11; i++){
            button = new JButton("Query " + (i+1));

            buttons.add(i,button);
            queries.add(button);
        }
    }

    /**
     * Método que gere a apresentação de um painel onde introduzir o input para uma query
     * @param panel O painel a apresentar
     */
    public void setInput(JPanel panel){
        if(input != null) remove(input);
        if(resposta != null) remove(resposta);
        input = panel;
        this.add(input);
        this.validate();
    }
}
