import java.util.Scanner;
import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.TreeSet;
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.util.stream.Collectors;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.InputMismatchException;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;


/**
 * Classe que funciona como menu da aplicacao e faz mariotariamente todo o input/output do programa.
 */

public class JavaFatura extends Exception
{
    
    /**
     * Construtor sem argumentos.
     */
    
    private final static String adminpassword = "admin";
    /**
      * Metodo que imprime o menu com base no submenu que esta.
      * @param i         Submenu atual.
    */
    private static void imprimirmenu(int i){
        //System.out.print('\u000C');
        if (i == 1)
            imprimirmenu1();
        if (i == 2)
            imprimirmenu2();
        if (i == 3)
            imprimirmenu3();
        if (i == 4)
            imprimirmenu4();
        if (i == 5)
            imprimirmenu5();
        if (i == 6)
            imprimirmenu6();
        
            
    }
    /**
      * Metodo que imprime o submenu 1.
    */
    private static void imprimirmenu1(){
        System.out.println("------------------Java Fatura------------------");
        System.out.println("Opçoes");
        System.out.println("1-Registar contribuintes");
        System.out.println("2-Fazer Login");        
        System.out.println("3-Guardar Ficheiro");
        System.out.println("4-Importar Ficheiro");
        System.out.println("5-Imprimir Conselhos bonificados");
        System.out.println("6-Sair");
    }
    /**
      * Metodo que imprime o submenu 2.
    */
    private static void imprimirmenu2(){
        System.out.println("------------------Registar contribuintes------------------");
        System.out.println("Opçoes");
        System.out.println("1-Registar Contribuinte Individual");
        System.out.println("2-Registar Empresa");
        System.out.println("3-Voltar Menu anterior");
    }
    /**
      * Metodo que imprime o submenu 3.
    */
    private static void imprimirmenu3(){
        System.out.println("------------------Menu de Login------------------");
        System.out.println("Opçoes");
        System.out.println("1-Login de Contribuinte Individual");
        System.out.println("2-Login de Empresa");
        System.out.println("3-Login de Contribuinte Admin");
        System.out.println("4-Voltar Menu anterior");
    }
    /**
      * Metodo que imprime o submenu 4.
    */
    private static void imprimirmenu4(){
        System.out.println("------------------Menu de Empresas------------------");
        System.out.println("Opçoes");
        System.out.println("1-Criar faturas");
        System.out.println("2-Registo faturas");
        System.out.println("3-Faturas por contribuinte numda dada data");
        System.out.println("4-Faturas por contribuinte ");
        System.out.println("5-Total faturado num dado intervalo");
        System.out.println("6-Adicionar setor");
        System.out.println("7-Log out");
    }
    /**
      * Metodo que imprime o submenu 5.
    */
    private static void imprimirmenu5(){
        System.out.println("------------------Menu de Contribuinte Individual------------------");
        System.out.println("Opçoes");
        System.out.println("1-Listagem de faturas");
        System.out.println("2-Montagem fiscal do agregado");
        System.out.println("3-Adicionar agregado");
        System.out.println("4-Adicionar setores");
        System.out.println("5-Validar faturas");
        System.out.println("6-Montagem fiscal individual");
        System.out.println("7-Log out");
    }
    /**
      * Metodo que imprime o submenu 6.
    */
    private static void imprimirmenu6(){
        System.out.println("------------------Menu de ADMIN------------------");
        System.out.println("Opçoes");
        System.out.println("1-Relacao entre os 10 contribuintes que mais gastam");
        System.out.println("2-Relacao entre as X empresas que mais faturam");
        System.out.println("3-Validar Setores");
        System.out.println("4-Imprimir base de dados");        
        System.out.println("5-Log out");
    }
    
    /**
      * Metodo que insere uma fatura numa base de dados geral.
      * @param bd         Base de dados onde vai ser inserida o fatura.
      * @param emp        Empresa que cria a fatura.
    */
   
    private static void inserirFatura(BDgeral bd,Empresa emp) throws Erros{
        int int_1,int_2,int_3,int_4, int_5, int_6 = 0;
        double double_1=0;
        String string_1, string_2, string_3 = null;
        Scanner ac = new Scanner(System.in).useDelimiter("\\n");
        try{
                         System.out.println("NIF cliente: ");
                         int_2=ac.nextInt();
                         System.out.println("Descricao : ");
                         string_2=ac.next();
                         System.out.println("Valor : ");
                         double_1 = ac.nextDouble();
                         
                         System.out.println("----Data---- ");
                         System.out.println("Dia: ");
                         int_4=ac.nextInt();
                         System.out.println("Mes: ");
                         int_5=ac.nextInt();
                         System.out.println("Ano: ");
                         int_6=ac.nextInt();
                         
        }
           catch(InputMismatchException e){
           ac.next();
           throw new Erros("Erro no input passado");
        }
        
        Set<String> categorias = new TreeSet<>();
        categorias = emp.getSetores();
        if (categorias.size() == 1)
            for(String a : categorias)
                string_3 = a;
        else{
            string_3 = "";
        }
        Fatura faturaaux;
        try{
            faturaaux = new Fatura(emp.getNif(),emp.getNome(),LocalDate.of(int_6,int_5,int_4),int_2,string_2,string_3,double_1);
        }
        catch(java.time.DateTimeException e){
            System.out.println(e.getMessage());
            throw new Erros("Data invalida");
        }
        try{
            bd.addFatura(faturaaux);
        }
        catch(ErroNotFound l){
            throw new Erros ("O contribuinte " + l.getMessage() + " não está inserido.");
        }
    }
    
    /**
      * Metodo que insere uma empresa numa base de dados geral.
      * @param bd         Base de dados onde vai ser inserida a empresa.
    */
    private static void inserirEmpresa(BDgeral bd) throws Erros, ErroNotFound, ErroContains{
        int int_1,int_2,int_3,int_4, int_5, int_6, int_7 = 0;
        int_6 = 0;
        double double_1=0;
        String string_1, string_2, string_3, string_4 = null;
        String string_5 = "teste";
        Scanner ac = new Scanner(System.in).useDelimiter("\\n");
        Set<Integer> faturas = new TreeSet<>();
        Set<String> setores = new TreeSet<>();
        Set<Integer> agregados = new TreeSet<>();
        try{
                         System.out.println("NIF da empresa : ");
                         int_1 = ac.nextInt();
                         System.out.println("Email: ");
                         string_1 = ac.next();
                         System.out.println("Nome: ");
                         string_2 = ac.next();
                         System.out.println("Morada: ");
                         string_3 = ac.next();
                         System.out.println("Password: ");
                         string_4 = ac.next();
                         System.out.println("Adicionar Setores (minimo 1 setor)");
                         while(!string_5.equals("done") || int_6 == 0 ){
                             System.out.println("Setor (done para terminar):");
                             string_5 = ac.next();
                             if (!string_5.equals("done")){
                                setores.add(string_5);
                                int_6++;
                            }
                             
                         }
                         System.out.println("Rendimento Anual: ");
                         double_1=ac.nextDouble();
                         
                         System.out.println("Conselho: ");
                         string_5=ac.next();
        }
           catch(InputMismatchException e){
           throw new Erros("Erro no input passado");
        }
        Empresa empresa_aux = null;
        if (bd.containsConcelho(string_5.toUpperCase()) ) {
            empresa_aux = new EmpInterior(int_1, string_1, string_2, string_3, string_4, setores,calculaBenE(double_1),faturas, string_5); 
        }
        else{   
            empresa_aux = new Empresa(int_1, string_1, string_2, string_3, string_4, setores,calculaBenE(double_1),faturas); 
        }
        
        try{
            bd.addEmpresa(empresa_aux);
        }
        catch(ErroNotFound e){
            throw e;
        }
        catch(ErroContains e){
            throw e;
        }
        
        
    }
    
    /**
      * Metodo que calcula a taxa de beneficiencia com base no rendimento anual.
      * @param i         Rendimento anual.
      * @return          Taxa de beneficiencia.
    */
    private static double calculaBenE(double i){
        if (i > 50000)
            i = 50000;
        if (i < 0)
            i = 0;
        i=-0.00001 * i + 0.5;
            
            
        return i;
    
    
    }
    
    /**
      * Metodo que insere um contribuinte individual numa base de dados geral da throw a Erros,ErroNotFound e ErroContains.
      * @param bd         Base de dados onde vai ser inserida o contribuinte individual.
    */
    private static void inserirCIndi(BDgeral bd) throws Erros, ErroNotFound, ErroContains{
        int int_1,int_2,int_3,int_4, int_5, int_6 = 0;
        int int_7 = 0;
        int n_agregado = 0;
        double double_1=0;
        String string_1, string_2, string_3, string_4, string_5 = "teste";
        Scanner ac = new Scanner(System.in).useDelimiter("\\n");
        Set<Integer> faturas = new TreeSet<>();
        Set<String> setores = new TreeSet<>();
        Set<Integer> agregados = new TreeSet<>();
        try{            
                         System.out.println("NIF : ");
                         int_1 = ac.nextInt();
                         System.out.println("Email: ");
                         string_1 = ac.next();
                         System.out.println("Nome: ");
                         string_2 = ac.next();
                         System.out.println("Morada: ");
                         string_3 = ac.next();
                         System.out.println("Password: ");
                         string_4 = ac.next();
                         
                         
                         
                         System.out.println("Nif do agregado");
                         while(n_agregado >= 0){
                             System.out.println("Próximo nif (numero negativo para parar de inserir elementos do agregado): ");
                             n_agregado = ac.nextInt();
                             if (n_agregado != -2 && n_agregado >= 0 && n_agregado != int_1)
                                agregados.add(n_agregado);
                         }
                         
                         
                         System.out.println("Adicionar Setores");
                         while(!string_5.equals("done")){
                             System.out.println("Setor (done para terminar):");
                             string_5 = ac.next();
                             if (!string_5.equals("done"))
                                setores.add(string_5);
                         }
        
                         System.out.println("Rendimento Anual: ");
                         double_1=ac.nextDouble();
                            
                         
                         
                         
           }
           catch(InputMismatchException e){
           throw new Erros("Erro no input passado");
        }
        
        CIndividual individual_aux = null;   
        if(agregados.size() < 4){            
            individual_aux = new CIndividual(int_1,string_1,string_2,string_3,string_4,agregados.size(), agregados, calculaBenE(double_1), setores, faturas);
        }
        else{         
            individual_aux = new FamiliaNum(int_1,string_1,string_2,string_3,string_4,agregados.size(), agregados, calculaBenE(double_1), setores, faturas);
        }
        try{
            bd.addIndividual(individual_aux);
        }
        catch(ErroNotFound e){
            throw e;
        }
        catch(ErroContains e){
            throw e;
        }
        
    
    }
    
    /**
      * Metodo que tenta fazer login de uma empresa, da throw a ErroNotFound caso falhe.
      * @param bd         Base de dados onde vai ser feito o login.
      * @param nif        Nif da empresa.
      * @param passe      Password da empresa.
      * @return           Clasee empresa caso o login seja validado.
    */
    private static Empresa loginEmpresa(BDgeral bd, int nif, String passe) throws ErroNotFound{
        Empresa aux;
        try {
            aux = bd.getEmpresa(nif);
        }
        
        catch(ErroNotFound e){
            throw new ErroNotFound("Nif inexistente");
        }
        
        if (passe.equals(adminpassword))
                return aux;
                
        try{
            bd.getEmpresa(nif).trylogin(passe);
        }
        
        catch(ErroNotFound e){
            throw new ErroNotFound("Passe errada");
        }
    
        return aux;
        
    }
    
    /**
      * Metodo que tenta fazer login de um contribuinte individual, da throw a ErroNotFound caso falhe.
      * @param bd         Base de dados onde vai ser feito o login.
      * @param nif        Nif da contribuinte individual.
      * @param passe      Password da contribuinte individual.
      * @return           Classe contribuinte individual em caso de sucesso
    */
    private static CIndividual loginIndividual(BDgeral bd, int nif, String passe) throws ErroNotFound{
        CIndividual aux;
        try {
            aux = bd.getCIndividual(nif);
        }
        
        catch(ErroNotFound e){
            throw new ErroNotFound("Nif inexistente");
        }
    
        try{
            if (passe.equals(adminpassword))
                return aux;
            bd.getCIndividual(nif).trylogin(passe);
        }
        
        catch(ErroNotFound e){
            throw new ErroNotFound("Passe errada");
        }
    
        return aux;
            
    }
    /**
      * Metodo que tenta imprime o menu e cria interacao com o utilizador .
    */
    public static void menu()
    {
        Scanner ac = new Scanner(System.in);
        int flag = 1;
        int choice= -1;
        BDgeral bd = new BDgeral(); 
        
        
        int int_1,int_2,int_3,int_4,int_5, int_6 = 0;
        long long1,long2 = 0;
        double  double2 = 0;
        double double_1 = 0;
        String string_1,string_2 ,string_3= null;
        Fatura faturaaux = null;
        Empresa empresa_atual = null;
        CIndividual individual_atual = null;
        
        
        while(flag != 0){
            imprimirmenu(flag);
            
            try{
                choice=ac.nextInt(); 
            }
            catch(InputMismatchException e){
                System.out.println("Insere digitos apenas");
                ac.next();
               
                continue;
            }
            
        if (flag == 1){
                
            switch(choice){
                case 1:
                    flag = 2;
                    break;
                    
                case 2:
                    flag = 3;
                    break;
                    
                case 3:
                    try{
                        System.out.println("Nome Ficheiro : ");
                        string_1=ac.next();
                        bd.guardaEstado(string_1);                        
                    }
                    catch(InputMismatchException e){
                        System.out.println(e.getMessage());
                        break;
                    }
                    catch(FileNotFoundException e){
                        System.out.println(e.getMessage());
                        break;  
                    }
                    catch(IOException e){
                        System.out.println(e.getMessage());
                        break;  
                    }
                    System.out.println("Gravado com sucesso");
                    break;
                case 4:
                    try{
                        System.out.println("Nome Ficheiro : ");
                        string_1=ac.next();
                        bd.carregaEstado(string_1);                        
                    }
                    catch(InputMismatchException e){
                        System.out.println(e.getMessage());
                        break;
                    }
                    catch(FileNotFoundException e){
                        System.out.println(e.getMessage());
                        break;  
                    }
                    catch(IOException e){
                       System.out.println(e.getMessage());
                       break;  
                    }
                    catch(ClassNotFoundException e){
                        System.out.println(e.getMessage());
                        break;  
                    }
                    System.out.println("Lido com sucesso");
                    break;
               
                case 5:
                    System.out.println(bd.getConcelhos().toString());
                    break;
                
                case 6:
                    System.out.println("A sair");
                    flag = 0;
                    break;
                
                default:
                    System.out.println("Opçao Invalida");
                }
        
            }
        else if (flag == 2){
            switch(choice){
                case 2:
                    try{
                        inserirEmpresa(bd);
                    }
                    catch(Erros e){
                        System.out.println(e.getMessage());
                        break;
                    }
                    catch(ErroNotFound e){
                        System.out.println("NIF " + e.getMessage() + " nao encontrado");
                        break;
                    }
                    catch(ErroContains e){
                        System.out.println("NIF ja registado na base de dados");
                        break;
                    }
                    System.out.println("Inserido com sucesso");
                    break;
                    
                case 1:
                    try{
                        inserirCIndi(bd);
                    }
                    catch(Erros e){
                        System.out.println(e.getMessage());
                        break;
                    }
                    catch(ErroNotFound e){
                        System.out.println("NIF " + e.getMessage() + " nao encontrado");
                        break;
                    }
                    catch(ErroContains e){
                        System.out.println("NIF ja registado na base de dados");
                        break;
                    }
                    System.out.println("Inserido com sucesso");
                    break;
                    
                case 3:
                    flag = 1;
                    break;
                default:
                    System.out.println("Opçao Invalida");
                }
        }
        else if (flag == 3){
            switch(choice){
                case 1:
                
                    System.out.println("NIF: ");
                    try{
                        int_1 = ac.nextInt();
                        System.out.println("Password: ");
                        string_1 = ac.next();
                    
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    try{
                        individual_atual = loginIndividual(bd,int_1,string_1);
                    }
                    catch(ErroNotFound e){
                        System.out.println(e.getMessage());
                        break;
                    }
                    flag = 5;
                    break;
                    
                    
                    
                case 2:
                    System.out.println("NIF: ");
                    try{
                        int_1 = ac.nextInt();
                        System.out.println("Password: ");
                        string_1 = ac.next();
                    
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    try{
                        empresa_atual = loginEmpresa(bd,int_1,string_1);
                    }
                    catch(ErroNotFound e){
                        System.out.println(e.getMessage());
                        break;
                    }
                    flag = 4;
                    break;
                    
                case 3:
                    try{        
                        System.out.println("Password do admin: ");
                        string_1 = ac.next();
                    
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    if (string_1.equals(adminpassword)){
                        flag = 6;
                    }
                    else{
                        System.out.println("Password errada");
                    }
                        
                    break;
                
                case 4:
                    flag = 1;
                    break;
                default:
                    System.out.println("Opçao Invalida");
                }
        }
        
        else if (flag == 4){
            switch(choice){
                case 1:
                try{
                    inserirFatura(bd,empresa_atual);
                }
                catch(Erros e){
                    System.out.println(e.getMessage());
                    break;
                 }
                 System.out.println("Inserido com sucesso");
                 break;
                 
                case 2:
                    try{
                         System.out.println("Ordenar por data?");
                         
                         System.out.println("1 - Sim , outro numero - Ordenar por valor");
                         int_1=ac.nextInt();
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    if (int_1 == 1) 
                        System.out.println(bd.listagem_ordenada_emp_fatura(true,empresa_atual.getNif()));
                    else
                        System.out.println(bd.listagem_ordenada_emp_fatura(false,empresa_atual.getNif()));

                    break;
                 
                case 3:
                    try{
                         System.out.println("----Data inicial---- ");
                         System.out.println("Dia: ");
                         int_1=ac.nextInt();
                         System.out.println("Mes: ");
                         int_2=ac.nextInt();
                         System.out.println("Ano: ");
                         int_3=ac.nextInt();
                         System.out.println("----Data final---- ");
                         System.out.println("Dia: ");
                         int_4=ac.nextInt();
                         System.out.println("Mes: ");
                         int_5=ac.nextInt();
                         System.out.println("Ano: ");
                         int_6=ac.nextInt();
                    
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    try{
                        System.out.println(bd.listagem_cont_fatura_time(LocalDate.of(int_3,int_2,int_1),LocalDate.of(int_6,int_5,int_4),empresa_atual.getNif()).toString()) ;
                    }
                    catch(java.time.DateTimeException e){
                        System.out.println(e.getMessage());
                    }
                    break;
                    
                case 4:
                    
                    try{
                       // System.out.println(bd.listagem_ordenada_emp_fatura(false,empresa_atual.getNif()).toString()) ;
                       System.out.println(bd.listagem_cont_fatura(empresa_atual.getNif()).toString()) ;
                    }
                    catch(java.time.DateTimeException e){
                        System.out.println(e.getMessage());
                    }
                    break;
                 
                case 5:
                    try{
                         System.out.println("----Data inicial---- ");
                         System.out.println("Dia: ");
                         int_1=ac.nextInt();
                         System.out.println("Mes: ");
                         int_2=ac.nextInt();
                         System.out.println("Ano: ");
                         int_3=ac.nextInt();
                         System.out.println("----Data final---- ");
                         System.out.println("Dia: ");
                         int_4=ac.nextInt();
                         System.out.println("Mes: ");
                         int_5=ac.nextInt();
                         System.out.println("Ano: ");
                         int_6=ac.nextInt();
                    
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    try{
                        System.out.println(Double.toString(bd.total_faturado(LocalDate.of(int_3,int_2,int_1),LocalDate.of(int_6,int_5,int_4),empresa_atual.getNif())));
                    }
                    catch(java.time.DateTimeException e){
                        System.out.println(e.getMessage());
                    }
                    break;
                 
                case 6:
                    try{
                         System.out.println("Setor a inserir(cancel para cancelar)   : ");
                         string_1=ac.next();
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    
                    if(string_1.toUpperCase().equals("CANCEL")){
                        System.out.println("Cancelado");
                        break;
                    }
                    
                    try{
                        bd.addSetor(empresa_atual.getNif(),string_1,false) ;
                        empresa_atual = bd.getEmpresa(empresa_atual.getNif()); // isto supostamente nunca da erro
                    }
                    catch( ErroNotFound e){
                        System.out.println("Contribuinte nao encontrado " + e.getMessage());
                        break;
                    }
                    System.out.println("Inserido com sucesso");
                    break;
                case 7:
                    flag = 1;
                    break;
                   
                default:
                    System.out.println("Opçao Invalida");
                }
        }
        else if (flag == 5){
            switch(choice){
                case 1:
                    System.out.println( bd.getFaturas_de_Id(individual_atual.getFaturas())    .toString());                   
                    break;
                    
                case 2:
                    
                    for(Integer i : individual_atual.getNifsAgregado() ){
                        try{
                            System.out.println(i.toString() + " : " + Double.toString(bd.deduz_montante_Individual(i)));
                        }
                        catch(ErroNotFound e){
                            System.out.println("Contribuinte nao encontrado " + e.getMessage());
                        }
                    }
                    
                    break;
                    
                case 3:
                    try{
                         System.out.println("NIF a inserir no agregado (-1 para cancelar): ");
                         int_1=ac.nextInt();
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    
                    if(int_1 == -1){
                        System.out.println("Cancelado");
                        break;
                    }
                        
                    
                    try{
                        bd.addAgregado(individual_atual.getNif(),int_1) ;
                        individual_atual = bd.getCIndividual(individual_atual.getNif()); // nao deve dar erro :/
                    }
                    catch( ErroNotFound e){
                        System.out.println("Contribuinte nao encontrado" + e.getMessage());
                        break;
                    }
                    catch( ErroContains l){
                        System.out.println("Contribuinte " + l.getMessage() + " ou não está inserido ou existe uma empresa com este nif.");
                        break;
                    }
                    catch( Erros l){
                        System.out.println("Contribuinte ja pertence ao agregado.");
                        break;
                    }
                    
                    System.out.println("Inserido com sucesso");
                    break;
                   
                case 4:
                    try{
                         System.out.println("Setor a inserir (cancel para cancelar): ");
                         string_1=ac.next();
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    
                    if(string_1.toUpperCase().equals("CANCEL")){
                        System.out.println("Cancelado");
                        break;
                    }
                    
                    try{
                        bd.addSetor(individual_atual.getNif(),string_1,true) ;
                        individual_atual = bd.getCIndividual(individual_atual.getNif()); // nao deve dar erro :/
                    }
                    catch( ErroNotFound e){
                        System.out.println("Contribuinte nao encontrado" + e.getMessage());
                        break;
                    }
                    System.out.println("Inserido com sucesso");
                    break;
                
                case 5:
                    bd.valida_faturas_contribuinte(individual_atual.getNif());
                    
                    break;
                case 6:
                    try{
                        System.out.println(Double.toString(bd.deduz_montante(individual_atual)));
                    }
                    catch(ErroComum l){
                        System.out.println("Classe passada como argumento não compatível.");
                    }
                    break;
                    
                case 7:
                    flag = 1;
                    break;
                default:
                    System.out.println("Opçao Invalida");
                
            
           }
        
        }
        
        else if (flag == 6){
            switch(choice){
                case 1:
                    System.out.println(bd.rel_top10().toString());
                    
                    break;
                case 2:
                    try{
                        System.out.println("Numero de empresas: ");
                        int_1=ac.nextInt();
                        System.out.println(bd.top_X_faturacao(int_1).toString());
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }

                    
                    
                    break;
                
                case 3:
                    try{
                        bd.Setores_admin();
                    }
                    catch(AdminAprov e){
                        System.out.println(e.getMessage()); 
                    }
                    break;
                    
                case 4:
                
                    System.out.println("1 - Imprimir base de dados toda ");
                    System.out.println("2 - Imprimir base de dados das faturas ");
                    System.out.println("3 - Imprimir base de dados das empresas ");
                    System.out.println("4 - Imprimir base de dados dos contribuintes ");
                    System.out.println("5 - Imprimir base de dados dos setores");
                    try{
                         int_1=ac.nextInt();
                    }
                    catch(InputMismatchException e){
                        System.out.println("Erro no input passado");
                        break;
                    }
                    switch(int_1){
                        case 1:
                            System.out.print(bd.toString());
                            break;
                        case 2:
                            System.out.print(bd.toStringFaturas());
                            break;
                        case 3:
                            System.out.print(bd.toStringEmpresas());
                            break;
                        case 4:
                            System.out.print(bd.toStringIndividuais());
                            break;
                        case 5:
                            System.out.print(bd.toStringSetores());
                            break;
                            
                        default:
                            System.out.println("Opçao Invalida");
                    }
                    
                    
                    break;
                    
                case 5:
                    flag = 1;
                    break;
                
                default:
                    System.out.println("Opçao Invalida");
                
            
           }
        
        }
    
    }
    System.out.println("Saiu com sucesso");
  
 }
}
