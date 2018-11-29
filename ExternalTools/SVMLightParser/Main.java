import java.io.*;
import java.util.*;

public class Main {

    public static void main(String[] args) throws IOException {
      File filein = new File("C:\\Users\\T\\IdeaProjects\\expert to SVM parser\\src\\review_output_svm.txt");
      File fileout = new File("C:\\Users\\T\\IdeaProjects\\expert to SVM parser\\src\\new_review_output_svm.txt");
      File fileforPyPos = new File("C:\\Users\\T\\IdeaProjects\\expert to SVM parser\\src\\new_review_output_py_pos.txt");
        File fileforPyNeg = new File("C:\\Users\\T\\IdeaProjects\\expert to SVM parser\\src\\new_review_output_py_neg.txt");
        if(!fileout.exists()){
            fileout.createNewFile();
        }
        if(!fileforPyPos.exists()){
            fileout.createNewFile();
        }
        if(!fileforPyNeg.exists()){
            fileout.createNewFile();
        }
      BufferedReader br = new BufferedReader(new FileReader(filein));
      BufferedWriter bw = new BufferedWriter(new FileWriter(fileout));
      BufferedWriter bwp = new BufferedWriter(new FileWriter(fileforPyPos));
      BufferedWriter bwn = new BufferedWriter(new FileWriter(fileforPyNeg));

      String st;
      int wordscount[] = new int[99999];
      int reviewnum=0;
      List<String> words= new ArrayList<String>();
      String[] arr;
      int step=0;
      final int minPosStars = 3;
      final int maxNegStars = 2;

      while((st = br.readLine())!=null){
          reviewnum++;
          String reviewOutput="";
          String reviewforPy="";
          boolean isPos=false;
          Arrays.fill(wordscount, 0);
          System.out.println();
                    if(Integer.parseInt(st)>=minPosStars){
                        bw.write(1+" ");
                        isPos=true;
                    }else if(Integer.parseInt(st)<=maxNegStars){
                        bw.write(-1+" ");
                    }
                    st=br.readLine();
                    if(isPos) {
                        bwp.write(st);
                        bwp.newLine();
                    }
                    else {
                        bwn.write(st);
                        bwn.newLine();
                    }
                    if(st.matches("end")){
                        break;
                    }
                    arr=st.split(" ");
                    for(String word : arr){
                        if(!words.contains(word)){
                            words.add(word);
                            wordscount[words.indexOf(word)]++;
                        }else{
                            wordscount[words.indexOf(word)]++;
                        }
                    }

                    for(int index = 0; index<wordscount.length;index++){
                        if(wordscount[index]>0){
                            reviewOutput+=((index+1) + ":"+(((float)wordscount[index])/(float)arr.length)+" ");

                        }

            }
          System.out.println(reviewOutput);
          bw.write(reviewOutput);
          bw.newLine();

      }
      bwp.close();
      bwn.close();
          bw.close();
          br.close();

    }
}
