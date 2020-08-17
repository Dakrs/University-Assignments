package TransferControl;

import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * Classe responsável por assegurar toda a informação necessária para executar leituras e escritas sobre ficheiros
 */


public class FileManager {
    private boolean [] chunks; // array de booleanos para as chunks já recebidas
    private int chunks_written; // chunks do ficheiro já recebidas
    private String file_name;
    private RandomAccessFile file_desc;

    /**
     * Construtor parameterizado utilizado pela entidade que envia o ficheiro
     * @param file_name Nome do ficheiro a enviar
     */

    public FileManager(String file_name){
        this.chunks_written = 0;
        this.file_name = file_name;
    }

    /**
     * Construtor parameterizado utilizado pela entidade que está a receber o ficheiro.
     * @param chunks Número de partições efetuadas sobre o ficheiro a receber
     * @param file_name Nome do ficheiro a receber
     */

    public FileManager(int chunks,String file_name){
        this.chunks_written = 0;
        this.chunks = new boolean[chunks];
        this.file_name = file_name;
    }

    /**
     * Método que cria o ficheiro para a entidade a receber.
     * @param chunks Número de partições sobre o ficheiro
     * @param l_size Tamanho da última partição
     */

    public void start(int chunks,int l_size){
        this.chunks = new boolean [chunks];
        try {
            this.file_desc = new RandomAccessFile("new_" + this.file_name, "rw");
            int aux = (chunks - 1 ) * 1024 * 4;
            byte[] inserir_chunk = new byte[1024*4];
            for(int i = 1; i < chunks ;i++){
                file_desc.write(inserir_chunk);
            }
            file_desc.write(new byte[l_size]);

        } catch (Exception e) {
            System.out.println("erromega xD");
        }
    }

    /**
     * Método responsável por, dados o número da partição e um conjunto de bytes, escrever os bytes para posição correta do ficheiro
     * @param chunk Número da partição
     * @param data Array com o bytes da informação recebida
     * @return Valor booleno para controlo de erros
     */

    private synchronized boolean writeData(int chunk, byte[] data) {
        int lower_limit = (4 * 1024) * chunk;
        int upper_limit = (4 * 1024) * (chunk + 1);
        boolean boll = true;

        try {

            if (lower_limit > file_desc.length()) {
                return false;
            }
            if (upper_limit > file_desc.length()) {

                upper_limit = (int) file_desc.length();
            }
            file_desc.seek(lower_limit);
            file_desc.write(data, 0, upper_limit - lower_limit);
        } catch (IOException e) {
            boll = false;
            System.out.println(data.length);
            e.printStackTrace();
            System.out.println("IOException at specific_range_of_bytes");
        }

        return boll;
    }

    /**
     * Método responsável por confirmar que um partição foi recebida e utilizado quando é recebido uma ACK do lado da entidade que envia
     * @param chunk Partição recebida
     * @return True caso a partição já tenha sido recebida, False caso seja a primeira vez que a recebe
     */

    public boolean addChunk(int chunk){
        if (!this.chunks[chunk]){
            this.chunks[chunk] = true;
            this.chunks_written++;

            return false;
        }
        return true;
    }

    /**
     * Método responsável por adicionar uma partição recebida ao ficheiro do lado da entidade que recebe o ficheiro
     * @param chunk Número da partição recebida
     * @param info Array de bytes da partição recebida
     * @return True caso a partição ainda não tenha sido recebida.
     */

    public synchronized boolean addChunk(int chunk,byte[] info){
        if (!this.chunks[chunk]){
            if (!writeData(chunk,info)) {
                System.out.println("Erro Mega Write Data");
                return false;
            }
            this.chunks[chunk] = true;
            this.chunks_written++;

            return true;
        }
        return false;
    }

    /**
     * Método que verifica que a transferência por ser fechada ou seja, verifica se todas as partições do ficheiro fora recebidas
     * @return Valor lógico da verificação
     */

    public synchronized boolean isFinished(){
        return this.chunks_written == this.chunks.length;
    }

    /**
     * Método que calcula quantas partições foram feitas.
     * @return Número de partições existentes.
     */

    public int getChunksLenght() {return this.chunks.length;}

    /**
     * Método que verifica se uma partição já foi recebida
     * @param chunk Número da partição a verificar
     * @return Valor lógico da verificação
     */

    public boolean checkChunk(int chunk) {return this.chunks[chunk];}

    /**
     * Método que calcula um array com o estado do ficheiro
     * @return Array de booleanos como o estado da partições. True caso a partição já tenha sido recebida, False caso contrário
     */

    public boolean[] getChunks() {
        return chunks;
    }

    /**
     * Metódo que calcula o número de partições recebidas até ao momento
     * @return Número de chunks recebidas até ao momento
     */

    public synchronized int getChunks_written() {
        return chunks_written;
    }

    /**
     * Método para fechar o escritor para o ficheiro.
     */

    public void writeFile(){
        try{
            file_desc.close();
        }
        catch (IOException e) {
            System.out.println("error while closing a file");
        }
    }
}
