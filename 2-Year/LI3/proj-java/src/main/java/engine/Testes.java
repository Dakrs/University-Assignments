package engine;

import java.lang.reflect.Array;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.List;
import java.util.Scanner;

/**
 * Classe com o simples intuito de testar o nosso programa
 */
public class Testes {

    /**
     * Função main que permite testar interativamente as queries do projeto
     */
    public static void main(String[] args) {
        long l, l2;
        double time;

        time = System.nanoTime();

        TCDCommunity com = new TCDCommunity();
        //com.load("/home/random/Desktop/dump/android/");
        com.load("/Users/Ambrosiny/Desktop/dump/ubuntu/");

        time = System.nanoTime() - time;

        System.out.println("Load done in " + time/1000000 + " milisecs");


        Scanner s = new Scanner(System.in);
        int n;
        String str;
        LocalDate begin, end;
        while(true) {
            try {
                n = s.nextInt();
            }
            catch (InputMismatchException e){
                n = 1000;
            }
            if(n == 1) {
                System.out.println("Post a verificar");
                l = s.nextLong();
                time = System.nanoTime();
                System.out.println(com.infoFromPost(l));
                time = System.nanoTime() - time;
                System.out.println("Query 1 done in " + time/1000000 + " milisecs");
            }
            if(n == 2) {
                System.out.println("Número de utilizadores a devlver");
                n = s.nextInt();
                time = System.nanoTime();
                List<Long> a = com.topMostActive(n);
                time = System.nanoTime() - time;
                System.out.println("Query 2 done in " + time/1000000 + " milisecs");
                //System.out.println(a);
            }
            if(n == 3) {
                System.out.println("Data de início do período:");
                begin = scanDate();
                System.out.println("Data de fim do período:");
                end = scanDate();
                time = System.nanoTime();
                System.out.println((com.totalPosts(begin, end)));
                time = System.nanoTime() - time;
                System.out.println("Query 3 done in " + time/1000000 + " milisecs");
            }
            if(n == 4) {
                s.nextLine();
                System.out.println("Inserir tag a verficar:");
                str = s.nextLine();
                System.out.println("Data de início do período:");
                begin = scanDate();
                System.out.println("Data de fim do período:");
                end = scanDate();
                time = System.nanoTime();
                System.out.println((com.questionsWithTag(str, begin, end)));
                time = System.nanoTime() - time;
                System.out.println("Query 4 done in " + time/1000000 + " milisecs");
            }
            if(n == 5) {
                System.out.println("User a verificar:");
                l = s.nextLong();
                time = System.nanoTime();
                System.out.println(com.getUserInfo(l));
                time = System.nanoTime() - time;
                System.out.println("Query 5 done in " + time/1000000 + " milisecs");
            }
            if(n == 6) {
                System.out.println("Número de utilizadores a devolver");
                n = s.nextInt();
                System.out.println("Data de início do período:");
                begin = scanDate();
                System.out.println("Data de fim do período:");
                end = scanDate();
                time = System.nanoTime();
                System.out.println(com.mostVotedAnswers(n, begin, end));
                time = System.nanoTime() - time;
                System.out.println("Query 6 done in " + time/1000000 + " milisecs");
            }
            if(n == 7) {
                System.out.println("Número de posts a devolver");
                n = s.nextInt();
                System.out.println("Data de início do período:");
                begin = scanDate();
                System.out.println("Data de fim do período:");
                end = scanDate();
                time = System.nanoTime();
                System.out.println(com.mostAnsweredQuestions(n, begin, end));
                time = System.nanoTime() - time;
                System.out.println("Query 7 done in " + time/1000000 + " milisecs");
            }
            if(n == 8) {
                System.out.println("Número de posts a devolver");
                n = s.nextInt();
                System.out.println("Palavra a verificar:");
                s.nextLine();
                str = s.nextLine();
                time = System.nanoTime();
                System.out.println(com.containsWord(n, str));
                time = System.nanoTime() - time;
                System.out.println("Query 8 done in " + time/1000000 + " milisecs");
            }
            if(n == 9) {
                System.out.println("Número de posts a devolver");
                n = s.nextInt();
                System.out.println("Primeiro user");
                l = s.nextLong();
                System.out.println("Segundo user");
                l2 = s.nextLong();
                time = System.nanoTime();
                System.out.println(com.bothParticipated(n, l, l2));
                time = System.nanoTime() - time;
                System.out.println("Query 9 done in " + time/1000000 + " milisecs");
            }
            if(n == 10) {
                System.out.println("Post a verificar");
                l = s.nextLong();
                time = System.nanoTime();
                System.out.println(com.betterAnswer(l));
                time = System.nanoTime() - time;
                System.out.println("Query 10 done in " + time/1000000 + " milisecs");
            }
            if(n == 11) {
                System.out.println("Número de users e tags a verificar");
                n = s.nextInt();
                System.out.println("Data de início do período:");
                begin = scanDate();
                System.out.println("Data de fim do período:");
                end = scanDate();
                time = System.nanoTime();
                System.out.println(com.mostUsedBestRep(n, begin, end));
                time = System.nanoTime() - time;
                System.out.println("Query 11 done in " + time/1000000 + " milisecs");
            }

            if (n == 0)
                break;
        }

    }

    /**
     * Função utilitária que gere o input de uma data por parte do utilizador
     */
    public static LocalDate scanDate(){
        int a, m, d;
        Scanner s = new Scanner(System.in);
        System.out.println("Inserir ano: ");
        a = s.nextInt();
        System.out.println("Inserir mês: ");
        m = s.nextInt();
        System.out.println("Inserir dia: ");
        d = s.nextInt();
        return LocalDate.of(a,m,d);
    }
}
