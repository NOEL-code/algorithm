import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;

public class BinSearch3 {

    static class PhysicData {
        private String name;
        private int height;
        private double vision;

        public PhysicData(String name, int height, double vision){
            this.name = name;
            this.height = height;
            this.vision = vision;
        }

        public String toString(){
            return name + " " + height + " " + vision;
        }

        public static final Comparator<PhysicData> Height_Order = new HeightOrderComparator();

        private static class HeightOrderComparator implements Comparator<PhysicData>{
            public int compare(PhysicData d1, PhysicData d2){
                return (d1.height > d2.height) ? 1 : (d1.height < d2.height) ? -1 : 0;
            }
        }

        public static void main(String []args){

            Scanner scan = new Scanner(System.in);
            PhysicData[] x = {
                    new PhysicData("a", 160, 1),
                    new PhysicData("b", 161, 2),
                    new PhysicData("c", 162, 3),
                    new PhysicData("d", 163, 4),
                    new PhysicData("e", 164, 5),
                    new PhysicData("f", 165, 6),
            };
            System.out.print("몇 cm를 찾고 있나요?: ");
            int height = scan.nextInt();
            int index = Arrays.binarySearch(x, new PhysicData("", height, 0.0), PhysicData.Height_Order);

            if(index < 0)
                System.out.println("요소가 없습ㄴ디ㅏ.");
            else {
                System.out.println("x[" + index + "]에 있습니다.");
                System.out.println("찾은 데이터: " + x[index].toString());
            }
        }
    }
}
