package li3;

import engine.TCDCommunity;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.time.LocalDate;


/**
 * Classe onde está definido o comportamento do modelo MVC que utiliza TCDCommunity como model, e View como view
 */
public class Controller {
    private TCDCommunity model = null;
    private View view = null;
    private String path;

    /**
     * Construtor sem parâmetros da classe Controller
     */
    public Controller() {
        this.view = new View();
    }

    /**
     * Construtor parametrizado da classe Controller
     * @param model Model do novo controller
     * @param view View do novo controller
     */
    public Controller(TCDCommunity model, View view) {
        this.model = model;
        this.view = view;

        this.view.addPathListener(new PathListener());
        view.setLocationRelativeTo(null);
        view.setVisible(true);


    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que especifica a localização da dump
     */
    public class PathListener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent) {

            path = view.getPath();

            File dir = new File(path);
            String[] directories = dir.list(new FilenameFilter() {
                @Override
                public boolean accept(File current, String name) {
                    return new File(current, name).isDirectory();
                }
            });

            view.addDumps(directories);
            view.addDumpListener(new DumpListener());
            }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que especifica a dump a utilizar
     */
    public  class DumpListener implements  ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent) {
            view.isLoading();
            if(model != null) model.clear();
            model.load(path + view.getSelectedDump() + "/");
            view.mainMenu();

            view.addQuery1Listener(new Query1Listener());
            view.addQuery2Listener(new Query2Listener());
            view.addQuery3Listener(new Query3Listener());
            view.addQuery4Listener(new Query4Listener());
            view.addQuery5Listener(new Query5Listener());
            view.addQuery6Listener(new Query6Listener());
            view.addQuery7Listener(new Query7Listener());
            view.addQuery8Listener(new Query8Listener());
            view.addQuery9Listener(new Query9Listener());
            view.addQuery10Listener(new Query10Listener());
            view.addQuery11Listener(new Query11Listener());
        }

    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 1
     */
    public class Query1Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 1, a partir do Id de um post, obtém-se o seu título e o criador do tópico aque o post pertence. Para isso, introduza um Id válido (número positivo):\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label = new JLabel("Id: ");
            JTextField input_id= new JTextField();
            input_id.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label.setPreferredSize(new Dimension(30,25));

            input_id.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    view.answer(model.infoFromPost(Long.parseLong(input_id.getText())).toString());
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label);
            input.add(input_id);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 2
     */
    public class Query2Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 2, obtém-se uma lista com os N users mais ativos (com mais posts) na comunidade. Para isso, introduza um número válido (número positivo):\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label = new JLabel("Número de users: ");
            JTextField input_id= new JTextField();
            input_id.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label.setPreferredSize(new Dimension(130,25));

            input_id.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    view.answer(model.topMostActive(Integer.parseInt(input_id.getText())).toString());
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label);
            input.add(input_id);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 3
     */
    public class Query3Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 3, obtém-se um par com a contagem de perguntas e respostas feitas no período de tempo indicado (no formato (Nº de perguntas, Nº de respostas) ). Para isso, introduza duas datas no formato \"aaaa-mm-dd\":\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Data inicial: ");
            JLabel label2 = new JLabel("Data final: ");
            JTextField data1 = new JTextField("2000-01-01");
            JTextField data2 = new JTextField("2018-12-31");
            data1.setPreferredSize(new Dimension(100,25));
            data2.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(90,25));
            label2.setPreferredSize(new Dimension(90,25));

            data1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data2.getText() != "")
                        try{
                        view.answer(model.totalPosts(parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        }
                        catch( java.time.DateTimeException e){ System.out.println(e);}
                }

            });

            data2.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data1.getText() != "")
                        try{
                        view.answer(model.totalPosts(parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        }
                        catch( java.time.DateTimeException e){ System.out.println(e);}

                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(data1);
            input.add(label2);
            input.add(data2);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 4
     */
    public class Query4Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 4, bla bla bla bla. Para isso, introduza duas datas no formato \"aaaa-mm-dd\" e uma tag a procurar:\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Data inicial: ");
            JLabel label2 = new JLabel("Data final: ");
            JLabel label3 = new JLabel("Tag: ");
            JTextField data1 = new JTextField("2000-01-01");
            JTextField data2 = new JTextField("2018-12-31");
            JTextField tag = new JTextField(view.getTitle());
            data1.setPreferredSize(new Dimension(100,25));
            data2.setPreferredSize(new Dimension(100,25));
            tag.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(100,25));
            label2.setPreferredSize(new Dimension(100,25));
            label3.setPreferredSize(new Dimension(40,25));

            data1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data2.getText() != "" && tag.getText() != "")
                        try{
                            view.answer(model.questionsWithTag(tag.getText(),parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        }
                        catch(java.time.DateTimeException  e){ System.out.println(e);}


                }
            });

            data2.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data1.getText() != "" && tag.getText() != "")
                        try{
                            view.answer(model.questionsWithTag(tag.getText(),parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        }
                        catch( java.time.DateTimeException e){ System.out.println(e);}

                }

            });

            tag.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data1.getText() != "" && data2.getText() != "")
                        try {
                            view.answer(model.questionsWithTag(tag.getText(), parseDate(data1.getText()),parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                }

                });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(data1);
            input.add(label2);
            input.add(data2);
            input.add(label3);
            input.add(tag);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 5
     */
    public class Query5Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 5, a partir do Id de um user, obtém-se informação sobre o mesmo. Para isso, introduza um Id válido (número positivo):\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label = new JLabel("Id: ");
            JTextField input_id= new JTextField();
            input_id.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label.setPreferredSize(new Dimension(30,25));

            input_id.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    view.answer(model.getUserInfo(Long.parseLong(input_id.getText())).toString());
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label);
            input.add(input_id);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 6
     */
    public class Query6Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 6, calcula-se as N respostas mais votadas num período de tempo. Para isso, introduza duas datas no formato \"aaaa-mm-dd\" e o número de posts a devolver:\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Data inicial: ");
            JLabel label2 = new JLabel("Data final: ");
            JLabel label3 = new JLabel("Nº de respostas: ");
            JTextField data1 = new JTextField("2000-01-01");
            JTextField data2 = new JTextField("2018-12-31");
            JTextField n = new JTextField("10");
            data1.setPreferredSize(new Dimension(90,25));
            data2.setPreferredSize(new Dimension(90,25));
            n.setPreferredSize(new Dimension(60,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(85,25));
            label2.setPreferredSize(new Dimension(90,25));
            label3.setPreferredSize(new Dimension(120,25));

            data1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data2.getText() != "" && n.getText() != "")
                        try {
                            view.answer(model.mostVotedAnswers(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }

                    }
                });

            data2.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data1.getText() != "" && n.getText() != "")
                        try {
                            view.answer(model.mostVotedAnswers(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                }

                });

            n.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data1.getText() != "" && data2.getText() != "")
                        try {
                            view.answer(model.mostVotedAnswers(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                }

                });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(data1);
            input.add(label2);
            input.add(data2);
            input.add(label3);
            input.add(n);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 7
     */
    public class Query7Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 7, calcula-se as N perguntas com mais respostas num período de tempo. Para isso, introduza duas datas no formato \"aaaa-mm-dd\" e o número de posts a devolver:\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Data inicial: ");
            JLabel label2 = new JLabel("Data final: ");
            JLabel label3 = new JLabel("Nº de respostas: ");
            JTextField data1 = new JTextField("2000-01-01");
            JTextField data2 = new JTextField("2018-12-31");
            JTextField n = new JTextField("10");
            data1.setPreferredSize(new Dimension(90,25));
            data2.setPreferredSize(new Dimension(90,25));
            n.setPreferredSize(new Dimension(60,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(85,25));
            label2.setPreferredSize(new Dimension(90,25));
            label3.setPreferredSize(new Dimension(120,25));

            data1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data2.getText() != "" && n.getText() != "")
                        try {
                            view.answer(model.mostAnsweredQuestions(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                }

                });

            data2.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data1.getText() != "" && n.getText() != "")
                        try {
                            view.answer(model.mostAnsweredQuestions(Integer.parseInt(n.getText()), parseDate(data1.getText()),parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                }

                });

            n.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if (data1.getText() != "" && data2.getText() != "")
                        try {
                            view.answer(model.mostAnsweredQuestions(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                }

                });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(data1);
            input.add(label2);
            input.add(data2);
            input.add(label3);
            input.add(n);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 8
     */
    public class Query8Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 8, obtém-se as N perguntas mais recentes cujo título contenha uma dada palavra. Para isso, introduza um número de perguntas a devolver e uma palavra a procurar:\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Nº de perguntas: ");
            JLabel label2 = new JLabel("Palavra a procurar: ");
            JTextField n = new JTextField("20");
            JTextField word = new JTextField(view.getTitle());
            n.setPreferredSize(new Dimension(100,25));
            word.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(120,25));
            label2.setPreferredSize(new Dimension(120,25));

            n.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(word.getText() != "")
                        view.answer(model.containsWord(Integer.parseInt(n.getText()), word.getText()).toString());
                }

            });

            word.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(word.getText() != "")
                        view.answer(model.containsWord(Integer.parseInt(n.getText()), word.getText()).toString());
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(n);
            input.add(label2);
            input.add(word);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 9
     */
    public class Query9Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 9, calcula-se as N perguntas mais recentes em que dois determinados users tenham participado. Para isso, introduza o número de respostas a devolver e os Ids dos users a procurar:\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Id 1: ");
            JLabel label2 = new JLabel("Id 2: ");
            JLabel label3 = new JLabel("Nº max de perguntas: ");
            JTextField id1 = new JTextField("87");
            JTextField id2 = new JTextField("5691");
            JTextField n = new JTextField("10");
            id1.setPreferredSize(new Dimension(90,25));
            id2.setPreferredSize(new Dimension(90,25));
            n.setPreferredSize(new Dimension(60,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(50,25));
            label2.setPreferredSize(new Dimension(50,25));
            label3.setPreferredSize(new Dimension(150,25));

            id1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(id2.getText() != "" && n.getText() != "")
                        view.answer(model.bothParticipated(Integer.parseInt(n.getText()),Long.parseLong(id1.getText()), Long.parseLong(id2.getText())).toString());
                }

            });

            id2.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(id1.getText() != "" && n.getText() != "")
                        view.answer(model.bothParticipated(Integer.parseInt(n.getText()),Long.parseLong(id1.getText()), Long.parseLong(id2.getText())).toString());
                }

            });

            n.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(id1.getText() != "" && id2.getText() != "")
                        view.answer(model.bothParticipated(Integer.parseInt(n.getText()),Long.parseLong(id1.getText()), Long.parseLong(id2.getText())).toString());
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(id1);
            input.add(label2);
            input.add(id2);
            input.add(label3);
            input.add(n);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 10
     */
    public class Query10Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 10, devolve-se o id da melhor resposta para uma determinada pergunta. Para isso, introduza um Id válido (número positivo):\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label = new JLabel("Id: ");
            JTextField input_id= new JTextField();
            input_id.setPreferredSize(new Dimension(100,25));
            texto.setPreferredSize(new Dimension(570,185));
            label.setPreferredSize(new Dimension(30,25));

            input_id.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    view.answer(String.valueOf(model.betterAnswer(Long.parseLong(input_id.getText()))));
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label);
            input.add(input_id);
            view.setInput(input);
        }
    }

    /**
     * Classe que define o comportamento a tomar quando é ativado o elemento da interface gráfica que escolhe a query 11
     */
    public class Query11Listener implements ActionListener{
        @Override
        public void actionPerformed(ActionEvent actionEvent){
            String s = "Na query 11, calcula-se as N tags mais utilizadas pelos N utilizadores com mais reputação, num dado intervalo. Para isso, introduza duas datas no formato \"aaaa-mm-dd\" e o número de tags (os seus Ids) a devolver:\n";
            view.setSize(600, 500);
            view.setLocationRelativeTo(null);
            JPanel input = new JPanel();
            input.setLayout(new FlowLayout(FlowLayout.LEFT,5,5));
            input.setPreferredSize(new Dimension(580, 250));

            JTextPane texto = new JTextPane();
            JLabel label1 = new JLabel("Data inicial: ");
            JLabel label2 = new JLabel("Data final: ");
            JLabel label3 = new JLabel("Nº de tags: ");
            JTextField data1 = new JTextField("2000-01-01");
            JTextField data2 = new JTextField("2018-12-31");
            JTextField n = new JTextField("10");
            data1.setPreferredSize(new Dimension(90,25));
            data2.setPreferredSize(new Dimension(90,25));
            n.setPreferredSize(new Dimension(60,25));
            texto.setPreferredSize(new Dimension(570,185));
            label1.setPreferredSize(new Dimension(85,25));
            label2.setPreferredSize(new Dimension(90,25));
            label3.setPreferredSize(new Dimension(120,25));

            data1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data2.getText() != "" && n.getText() != "") {
                        try {view.answer(model.mostUsedBestRep(Integer.parseInt(n.getText()),parseDate(data1.getText()), parseDate(data2.getText())).toString());}
                        catch( java.time.DateTimeException e ){ System.out.println(e);}
                    }
                }

            });

            data2.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data1.getText() != "" && n.getText() != "") {
                        try {
                            view.answer(model.mostUsedBestRep(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e ) {
                            System.out.println(e);
                        }
                    }
                }

            });

            n.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    if(data1.getText() != "" && data2.getText() != "") {
                        try {
                            view.answer(model.mostUsedBestRep(Integer.parseInt(n.getText()), parseDate(data1.getText()), parseDate(data2.getText())).toString());
                        } catch (java.time.DateTimeException e) {
                            System.out.println(e);
                        }
                    }
                }

            });

            texto.setEditable(false);
            texto.setText(s);
            input.add(texto);
            input.add(label1);
            input.add(data1);
            input.add(label2);
            input.add(data2);
            input.add(label3);
            input.add(n);
            view.setInput(input);
        }
    }

    /**
     * Método utilitário que efetua o parsing de uma data no formato "AAAA-MM-DD..."
     */
    public LocalDate parseDate(String str){
        if(str == null)
            return null;

        StringBuilder sb = new StringBuilder();
        int year, day, month;
        sb.append(str.charAt(0));
        sb.append(str.charAt(1));
        sb.append(str.charAt(2));
        sb.append(str.charAt(3));
        year = Integer.parseInt(sb.toString());
        sb.setLength(0);

        sb.append(str.charAt(5));
        sb.append(str.charAt(6));
        month = Integer.parseInt(sb.toString());
        sb.setLength(0);

        sb.append(str.charAt(8));
        sb.append(str.charAt(9));
        day = Integer.parseInt(sb.toString());

        LocalDate d = LocalDate.of(year, month, day);
        return d;
    }

}

